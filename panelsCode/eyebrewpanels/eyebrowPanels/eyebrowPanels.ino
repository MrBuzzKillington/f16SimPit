/*
 panels near the MFDS
 */
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//


unsigned long previousMillis_ = 0;        // will store last time LED was updated
unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;

//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 7;
byte consoleOutLevel_ = 0;

void onPriConsolesBrtKnbChange(unsigned int newValue) 
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 100);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);


//SWITCHES and buttons
DcsBios::ActionButton advModeSwToggle("ADV_MODE_SW", "TOGGLE", 13);
DcsBios::LED lightActive(0x447e, 0x0004, 12);
DcsBios::LED lightStby(0x447e, 0x0008, 11);
DcsBios::Switch2Pos altRelBtn("ALT_REL_BTN", 25);
//ECM LED????  pins 23
DcsBios::Switch3Pos rfSw("RF_SW", 31, 29);
DcsBios::Switch2Pos laserArmSw("LASER_ARM_SW", 27);
DcsBios::Switch3Pos masterArmSw("MASTER_ARM_SW", 22, 24,50,true);
DcsBios::Switch3Pos apPitchSw("AP_PITCH_SW", 32, 30,50,true);
DcsBios::Switch3Pos apRollSw("AP_ROLL_SW", 26, 28,50,true);

//RWR
DcsBios::Switch2Pos rwrHandoffBtn("RWR_HANDOFF_BTN", 35);
DcsBios::Switch2Pos rwrModeBtn("RWR_MODE_BTN", 37);
DcsBios::LED lightRwrModePri(0x4480, 0x0008, 39);
DcsBios::LED lightRwrModeOpen(0x4480, 0x0010, 41);
DcsBios::Switch2Pos rwrTBtn("RWR_T_BTN", 43);
DcsBios::LED lightRwrTgtsepUp(0x4480, 0x0080, 45);
DcsBios::Switch2Pos rwrLaunchBtn("RWR_LAUNCH_BTN", 34);
DcsBios::LED lightRwrMslLaunch(0x4480, 0x0004, 36);
DcsBios::LED lightRwrMslLaunch2(0x4480, 0x0004, 38);
DcsBios::Switch2Pos rwrSysTestBtn("RWR_SYS_TEST_BTN", 40);
DcsBios::LED lightRwrSystest(0x4480, 0x0040, 42);
DcsBios::Switch2Pos rwrUnknownShipBtn("RWR_UNKNOWN_SHIP_BTN", 44);
DcsBios::LED lightRwrShipUnk(0x4480, 0x0020, 46);


DcsBios::LED lightMasterCaution(0x447a, 0x0001, 14);
DcsBios::Switch2Pos masterCaution("MASTER_CAUTION", 15);
DcsBios::Switch2Pos fAckBtn("F_ACK_BTN", 16);
DcsBios::Switch2Pos iffIdBtn("IFF_ID_BTN", 17);
//
//Sync the game with the buttons
//
void PollAllControls()
{
    rfSw.resetThisState();
    laserArmSw.resetThisState();
    masterArmSw.resetThisState();
    apPitchSw.resetThisState();
    apRollSw.resetThisState();   
}

//
//This is the update counter, we can use this to force a sync between buttons and game
//
//void onUpdateCounterChange(unsigned int newValue) 
//{
//    if (newValue%25 == 0)
//    {
//        PollAllControls();     
//    }
//    lastUpdateCounter_ = newValue;
//}
//DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);



unsigned long uLastModelTimeS = 0xFFFFFFFF; // Start big, to ensure the first step triggers a resync

void onModTimeChange(char* newValue) {
  unsigned long currentModelTimeS = atol(newValue);

  if( currentModelTimeS < uLastModelTimeS )
  {
    if( currentModelTimeS > 20 )// Delay to give time for DCS to finish loading and become stable and responsive
    {
      //PollAllControls(); 
        digitalWrite(23, HIGH);
        delay(150);
        digitalWrite(23, LOW);
        delay(150);
        digitalWrite(23, HIGH);
        delay(150);
        digitalWrite(23, LOW);
        
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


void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1)
    {
      digitalWrite(23, HIGH);
      PollAllControls();
    } else {
      digitalWrite(23, LOW);
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
  powerOnTest();
  //Lights
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights

  DcsBios::setup();
  PollAllControls(); //sync the buttons at start
 
 }

void loop() 
{
  DcsBios::loop();
 
}

void powerOnTest()
{
        pinMode(23, OUTPUT); //ECM light
        pinMode(12, OUTPUT); //active light
        pinMode(11, OUTPUT); //stby light
        pinMode(39, OUTPUT); //pri light
        pinMode(41, OUTPUT); //open light
        pinMode(45, OUTPUT); //tgtStep light
        pinMode(36, OUTPUT); //mslLaunch light
        pinMode(38, OUTPUT); //msllaunch light
        pinMode(42, OUTPUT); //systest light
        pinMode(46, OUTPUT); //ship light

        int dlyTime = 100;
        digitalWrite(23, HIGH);
        delay(dlyTime);
        digitalWrite(12, HIGH);
        delay(dlyTime);
        digitalWrite(11, HIGH);
        delay(dlyTime);
        digitalWrite(39, HIGH);
        delay(dlyTime);
        digitalWrite(41, HIGH);
        delay(dlyTime);
        digitalWrite(45, HIGH);
        delay(dlyTime);
        digitalWrite(36, HIGH);
        delay(dlyTime);
        digitalWrite(38, HIGH);
        delay(dlyTime);
        digitalWrite(42, HIGH);
        delay(dlyTime);
        digitalWrite(46, HIGH);
        delay(1000);
        digitalWrite(23, LOW);
        delay(dlyTime);
        digitalWrite(12, LOW);
        delay(dlyTime);
        digitalWrite(11, LOW);
        delay(dlyTime);
        digitalWrite(39, LOW);
        delay(dlyTime);
        digitalWrite(41, LOW);
        delay(dlyTime);
        digitalWrite(45, LOW);
        delay(dlyTime);
        digitalWrite(36, LOW);
        delay(dlyTime);
        digitalWrite(38, LOW);
        delay(dlyTime);
        digitalWrite(42, LOW);
        delay(dlyTime);
        digitalWrite(46, LOW);
        delay(dlyTime);
}
