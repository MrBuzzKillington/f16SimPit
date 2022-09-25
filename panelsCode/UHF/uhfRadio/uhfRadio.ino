/*
 VHF Panel
 V1 - original
 v2 - fixed bug that caused screen to stay off.
 
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>

#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//

unsigned long previousMillis_ = 0;        // will store last time LED was updated
unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;

//Pins for uhf Radio freq and channel
const int uhf_bcd0Pin = 23;
const int uhf_bcd1Pin = 22;
const int uhf_bcd2Pin = 25;
const int uhf_bcd3Pin = 24;
const int numuhfLedLe = 8;
int uhfLedLePin[] = {26,27,28,29,31,30,32,33};
                  //{31,30,27,26,29,28,32,33};

int wentLightsOut_ = 1;
char lastUhf_[7];


DcsBios::RotaryEncoder uhfChanKnb("UHF_CHAN_KNB", "DEC", "INC", 35, 37);
DcsBios::RotaryEncoder uhfFreq0025Knb("UHF_FREQ_0025_KNB", "DEC", "INC", 51, 53);
DcsBios::RotaryEncoder uhfFreq01Knb("UHF_FREQ_01_KNB", "DEC", "INC", 47, 49);
DcsBios::RotaryEncoder uhfFreq1Knb("UHF_FREQ_1_KNB", "DEC", "INC", 43, 45);
DcsBios::RotaryEncoder uhfFreq10Knb("UHF_FREQ_10_KNB", "DEC", "INC", 39, 41);
DcsBios::Switch3Pos uhfFreq100Knb("UHF_FREQ_100_KNB", 34, 36);
DcsBios::Switch2Pos uhfStatusBtn("UHF_STATUS_BTN", 52);
DcsBios::Switch2Pos uhfTestBtn("UHF_TEST_BTN", 50);
DcsBios::Switch2Pos uhfSquelchSw("UHF_SQUELCH_SW", 7,true);
DcsBios::Switch2Pos uhfToneBtn("UHF_TONE_BTN", 6,true);
const byte uhfFuncKnbPins[4] = {38, 40, 42, 44};
DcsBios::SwitchMultiPos uhfFuncKnb("UHF_FUNC_KNB", uhfFuncKnbPins, 4);

DcsBios::Switch3Pos uhfModeKnb("UHF_MODE_KNB", 46, 48);
DcsBios::Potentiometer uhfVolKnb("UHF_VOL_KNB", A0);
//audio Panel
DcsBios::Switch3Pos comm1ModeKnb("COMM1_MODE_KNB", 8, 9);
DcsBios::Switch3Pos comm2ModeKnb("COMM2_MODE_KNB", 10, 11);
DcsBios::Potentiometer mslKnb("MSL_KNB", A1);
DcsBios::Potentiometer secVoiceKnb("SEC_VOICE_KNB", A2);
DcsBios::Potentiometer comm2PwrKnb("COMM2_PWR_KNB", A3);
DcsBios::Potentiometer comm1PwrKnb("COMM1_PWR_KNB", A4);
DcsBios::Potentiometer threatKnb("THREAT_KNB", A5);
DcsBios::Potentiometer tfKnb("TF_KNB", A6);
DcsBios::Potentiometer ilsPwrKnb("ILS_PWR_KNB", A7);
DcsBios::Potentiometer tacanKnb("TACAN_KNB", A8);
DcsBios::Potentiometer intercomKnb("INTERCOM_KNB", A9);

DcsBios::Switch3Pos hotMicSw("HOT_MIC_SW", 13, 12);
//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 2;
byte consoleOutLevel_ = 0;
void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 255, 0);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);


/*
 * 
 *   UHF RADIO LEDS
 *  
 * 
 */

int numCharToInt(char ledChar)
{
    if (ledChar == '0') { return 0; }
    if (ledChar == '1') { return 1; }
    if (ledChar == '2') { return 2; }
    if (ledChar == '3') { return 3; }
    if (ledChar == '4') { return 4; }
    if (ledChar == '5') { return 5; }
    if (ledChar == '6') { return 6; }
    if (ledChar == '7') { return 7; }
    if (ledChar == '8') { return 8; }
    if (ledChar == '9') { return 9; }
    if (ledChar == '*') { return 8; } //Test

    //Else
    return -1;
}
 
void setupUhfLED()
{
 //uhf LEDS
  pinMode(uhf_bcd0Pin, OUTPUT);
  pinMode(uhf_bcd1Pin, OUTPUT);
  pinMode(uhf_bcd2Pin, OUTPUT);
  pinMode(uhf_bcd3Pin, OUTPUT);
   
  //set direction of nLE (inverted)
  for (int thisLed = 0; thisLed < numuhfLedLe; thisLed++) 
  {
    pinMode(uhfLedLePin[thisLed], OUTPUT);
    digitalWrite(uhfLedLePin[thisLed], HIGH); //disable LE
  }
  //set defualt value
  for (int loopCnt = 0; loopCnt <9; loopCnt++)
  {
      for (int thisLed = 0; thisLed < numuhfLedLe; thisLed++) 
      {
        setUhfLedValue(thisLed, (thisLed+loopCnt)%8);
      }
       delay(500);
  }
}

void setUhfLedValue(int ledNum, int value)
{
    //Sets the bcd then sets the latch enable
    if (bitRead(value, 0))
    {
       digitalWrite(uhf_bcd0Pin, HIGH); 
    } else {
      digitalWrite(uhf_bcd0Pin, LOW);
    }
    if (bitRead(value, 1))
    {
       digitalWrite(uhf_bcd1Pin, HIGH); 
    } else {
      digitalWrite(uhf_bcd1Pin, LOW);
    }
    if (bitRead(value, 2))
    {
       digitalWrite(uhf_bcd2Pin, HIGH); 
    } else {
      digitalWrite(uhf_bcd2Pin, LOW);
    }
    if (bitRead(value, 3))
    {
       digitalWrite(uhf_bcd3Pin, HIGH); 
    } else {
      digitalWrite(uhf_bcd3Pin, LOW);
    }

    //set the proper latch enable
    for (int thisLed = 0; thisLed < numuhfLedLe; thisLed++) 
    {
      if (ledNum == thisLed)
      {
        digitalWrite(uhfLedLePin[thisLed], LOW);
        digitalWrite(uhfLedLePin[thisLed], HIGH);
      }
    }
}

void onUhfFreqDispChange(char* newValue) 
{
    //get each char
    setUhfLedValue(0,numCharToInt(newValue[6]));
    setUhfLedValue(1,numCharToInt(newValue[5]));
    setUhfLedValue(2,numCharToInt(newValue[4]));
    //decimal
    setUhfLedValue(3,numCharToInt(newValue[2]));
    setUhfLedValue(4,numCharToInt(newValue[1]));
    setUhfLedValue(5,numCharToInt(newValue[0]));
    //save off the last value we got
    for (int i=0;i<7;i++)
    {
      lastUhf_[i] = newValue[i];
    }   
}
DcsBios::StringBuffer<7> uhfFreqDispBuffer(0x45b2, onUhfFreqDispChange);



void onUhfChanDispChange(char* newValue) {
     setUhfLedValue(6,numCharToInt(newValue[0]));
     setUhfLedValue(7,numCharToInt(newValue[1]));
}
DcsBios::StringBuffer<2> uhfChanDispBuffer(0x45b0, onUhfChanDispChange);




void PollAllControls()
{
 //   emergStroreJett.resetThisState();
      uhfFreq100Knb.resetThisState();
      uhfStatusBtn.resetThisState();
      uhfTestBtn.resetThisState();
      uhfSquelchSw.resetThisState();
      uhfToneBtn.resetThisState();
      
      uhfFuncKnb.resetThisState();
      
      uhfModeKnb.resetThisState();
      uhfVolKnb.resetThisState();
      //audio Panel
      comm1ModeKnb.resetThisState();
      comm2ModeKnb.resetThisState();
      mslKnb.resetThisState();
      secVoiceKnb.resetThisState();
      comm2PwrKnb.resetThisState();
      comm1PwrKnb.resetThisState();
      threatKnb.resetThisState();
      tfKnb.resetThisState();
      ilsPwrKnb.resetThisState();
      tacanKnb.resetThisState();
      intercomKnb.resetThisState();
      hotMicSw.resetThisState();
}

void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1)
    {
      PollAllControls();
    } 
}
DcsBios::IntegerBuffer maxPwrSwBuffer(0x4424, 0x0008, 3, onMaxPwrSwChange);

/*
 * 
 *   MAIN FUNCTIONS
 * 
 * 
 */

// 
//  Main Functions
//
void setup() 
{
 
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  DcsBios::setup();
 
  //for the uhf leds
  setupUhfLED();
  
 }

void loop() 
{
  DcsBios::loop();
}

/*
 * 
 *   This code resysncs the pit when a game enters
 * 
 */
unsigned long uLastModelTimeS = 0xFFFFFFFF; // Start big, to ensure the first step triggers a resync

void onModTimeChange(char* newValue) {
  unsigned long currentModelTimeS = atol(newValue);

  if( currentModelTimeS < uLastModelTimeS )
  {
    if( currentModelTimeS > 20 )// Delay to give time for DCS to finish loading and become stable and responsive
    {
      //PollAllControls(); 
      DcsBios::resetAllStates();
      uLastModelTimeS = currentModelTimeS;
    }
  }
  else
  {
    uLastModelTimeS = currentModelTimeS;
  }
}
DcsBios::StringBuffer<6> modTimeBuffer(0x0440, onModTimeChange);
