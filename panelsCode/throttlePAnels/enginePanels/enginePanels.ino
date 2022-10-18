/*
 EnginePanels
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>

#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//




//Sensor Panel
//Electirc Panel
DcsBios::LED lightFlcsRly(0x447e, 0x0001, 3);
DcsBios::LED lightToFlcs(0x447c, 0x8000, 4);
DcsBios::LED lightFlFail(0x447e, 0x0020, 5);
DcsBios::LED lightEpuPmg(0x447c, 0x4000, 6);
DcsBios::LED lightEpuGen(0x447c, 0x2000, 7);
DcsBios::LED lightStbyGen(0x447c, 0x0800, 8);
DcsBios::LED lightMainGen(0x447c, 0x0400, 9);
DcsBios::LED lightFlcsPmg(0x447c, 0x0200, 10);
DcsBios::Switch2Pos elecCaution("ELEC_CAUTION", 11);
DcsBios::Switch3Pos mainPwrSw("MAIN_PWR_SW", 12, 13);

//EPU
DcsBios::LED lightEpu(0x447c, 0x0100, 44);
DcsBios::LED lightHydrazn(0x447c, 0x0040, 48);
DcsBios::LED lightAir(0x447c, 0x0080, 46);
DcsBios::Switch3Pos epuSw("EPU_SW", 50, 52,150);
DcsBios::Switch2Pos epuSwCoverOff("EPU_SW_COVER_OFF", 50); //tied to swtich no lever
DcsBios::Switch2Pos epuSwCoverOn("EPU_SW_COVER_ON", 52); //tied to swtich no lever
DcsBios::Switch2Pos epuSwCoverOff2("EPU_SW_COVER_OFF", 50,false,250); //tied to swtich no lever
DcsBios::Switch2Pos epuSwCoverOn2("EPU_SW_COVER_ON", 52,false,250); //tied to swtich no lever


//enigneSTartPanel
DcsBios::LED lightJfsRun(0x447c, 0x0020, 14);
DcsBios::Switch2Pos manualPitchSw("MANUAL_PITCH_SW", 15);
DcsBios::Switch3Pos jfsSw("JFS_SW", 16, 17);

DcsBios::Switch3Pos abResetSw("AB_RESET_SW", 19, 20);
//hoakey way to get the swith cover to toggle
DcsBios::Switch2Pos engContSwCover("ENG_CONT_SW_COVER", 18);
DcsBios::Switch2Pos engContSw("ENG_CONT_SW", 18,false,150);  //delay 100ms
DcsBios::Switch2Pos engContSwCover2("ENG_CONT_SW_COVER", 18,false,200);//dealy50ms
DcsBios::Switch2Pos maxPwrSw("MAX_PWR_SW", 21);

//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 2;


byte consoleOutLevel_ = 0;
void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 100);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights

}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);


void timeoutlightsOut( ) 
{
       consoleOutLevel_ = 0;
       analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
       //Turn off all the leds
       digitalWrite(3, LOW); //lightFlcsRly
       digitalWrite(4, LOW); //lightToFlcs
       digitalWrite(5, LOW); //lightFlFail
       digitalWrite(6, LOW); //lightEpuPmg
       digitalWrite(7, LOW); //lightEpuGen
       digitalWrite(8, LOW); //lightStbyGen
       digitalWrite(9, LOW); //lightMainGen
       digitalWrite(10, LOW); //lightFlcsPmg
       digitalWrite(44, LOW); //lightEpu
       digitalWrite(46, LOW); //lightHydrazn
       digitalWrite(48, LOW); //lightAir
       digitalWrite(14, LOW); //lightJfsRun    

    
 }

void PollAllControls()
{
 
    elecCaution.resetThisState();
    mainPwrSw.resetThisState();  
    epuSw.resetThisState();
    manualPitchSw.resetThisState();
    jfsSw.resetThisState();
    engContSw.resetThisState();
    abResetSw.resetThisState();
    engContSwCover.resetThisState();
  
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
  powerOnTest();
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  DcsBios::setup();  
  PollAllControls();
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


void powerOnTest()
{
     
        pinMode(3, OUTPUT); 
        pinMode(4, OUTPUT); 
        pinMode(5, OUTPUT); 
        pinMode(6, OUTPUT); 
        pinMode(7, OUTPUT); 
        pinMode(8, OUTPUT); 
        pinMode(9, OUTPUT); 
        pinMode(10, OUTPUT); 
        pinMode(44, OUTPUT); 
        pinMode(46, OUTPUT); 
        pinMode(48, OUTPUT); 
        pinMode(14, OUTPUT); 
        


        int dlyTime = 100;
        digitalWrite(3, HIGH);
        delay(dlyTime);
        digitalWrite(4, HIGH);
        delay(dlyTime);
        digitalWrite(5, HIGH);
        delay(dlyTime);
        digitalWrite(6, HIGH);
        delay(dlyTime);
        digitalWrite(7, HIGH);
        delay(dlyTime);
        digitalWrite(8, HIGH);
        delay(dlyTime);
        digitalWrite(9, HIGH);
        delay(dlyTime);
        digitalWrite(10, HIGH);
        delay(dlyTime);
        digitalWrite(44, HIGH);
        delay(dlyTime);
        digitalWrite(46, HIGH);
        delay(dlyTime);
        digitalWrite(48, HIGH);
        delay(dlyTime);
        digitalWrite(14, HIGH);
        delay(2000);
        digitalWrite(3, LOW);
        delay(dlyTime);
        digitalWrite(4, LOW);
        delay(dlyTime);
        digitalWrite(5, LOW);
        delay(dlyTime);
        digitalWrite(6, LOW);
        delay(dlyTime);
        digitalWrite(7, LOW);
        delay(dlyTime);
        digitalWrite(8, LOW);
        delay(dlyTime);
        digitalWrite(9, LOW);
        delay(dlyTime);
        digitalWrite(10, LOW);
        delay(dlyTime);
        digitalWrite(44, LOW);
        delay(dlyTime);
        digitalWrite(46, LOW);
        delay(dlyTime);
        digitalWrite(48, LOW);
        delay(dlyTime);
        digitalWrite(14, LOW);
        delay(2000);
}
