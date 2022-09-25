/*
 EnginePanels
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>
#include "LedControl.h"

#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//

unsigned long previousMillis_ = 0;        // will store last time LED was updated
unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;
int wentLightsOut_ = 1;
char lastcmds_[7];
bool lastWheelLight0_;
bool lastWheelLight1_;
bool lastWheelLight2_;


DcsBios::Switch2Pos emergStroreJett("EMERG_STRORE_JETT", 8);
DcsBios::Switch3Pos antiSkidSw("ANTI_SKID_SW", 9, 10);
DcsBios::Switch2Pos hookSw("HOOK_SW", 14);

DcsBios::Switch2Pos gndJettEnableSw("GND_JETT_ENABLE_SW", 15);
DcsBios::Switch2Pos brakeChanSw("BRAKE_CHAN_SW", 16);
DcsBios::Switch2Pos storesConfigSw("STORES_CONFIG_SW", 17);
DcsBios::Switch2Pos hornSilenceBtn("HORN_SILENCE_BTN", 18);
DcsBios::Switch3Pos landTaxiLightSw("LAND_TAXI_LIGHT_SW", 20, 19);
DcsBios::Switch2Pos dnLockBtn("DN_LOCK_BTN", 21);
DcsBios::Switch2Pos gearHandle("GEAR_HANDLE", A7);
DcsBios::LED lightGearWarn(0x447c, 0x0002, A6);
DcsBios::LED lightGearN(0x447a, 0x4000, 13);
DcsBios::LED lightGearL(0x447a, 0x8000, 12); //if we tied each leg 
DcsBios::LED lightGearR(0x447c, 0x0001, 11); //if we tied each leg

//RWR
DcsBios::Switch2Pos rwrActPwrBtn("RWR_ACT_PWR_BTN", 23);
DcsBios::Switch2Pos rwrSearchBtn("RWR_SEARCH_BTN", 25);
DcsBios::LED lightRwrSearch(0x447e, 0x0400, 27);
DcsBios::Switch2Pos rwrAltBtn("RWR_ALT_BTN", 29);
DcsBios::LED lightRwrAltLow(0x447e, 0x2000, 31);
DcsBios::ToggleButton rwrPwrBtnToggle("RWR_PWR_BTN", "0","1", 33);
//DcsBios::Switch2Pos rwrPwrBtn("RWR_PWR_BTN", 33);
DcsBios::LED lightRwrPower(0x447e, 0x8000, 35);
DcsBios::RotaryEncoder rwrIndDimKnb("RWR_IND_DIM_KNB", "-1024", "+1024", 39, 37);

//Need to save light state
void onLightGearNChange(unsigned int newValue) {
    if (newValue == 0)
    {
      lastWheelLight2_ = false;
    } else {
      lastWheelLight2_ = true;
    }
}
DcsBios::IntegerBuffer lightGearNBuffer(0x447a, 0x4000, 14, onLightGearNChange);
void onLightGearRChange(unsigned int newValue) {
    if (newValue == 0)
    {
      lastWheelLight0_ = false;
    } else {
      lastWheelLight0_ = true;
    }
}
DcsBios::IntegerBuffer lightGearRBuffer(0x447c, 0x0001, 0, onLightGearRChange);

void onLightGearLChange(unsigned int newValue) {
    if (newValue == 0)
    {
      lastWheelLight1_ = false;
    } else {
      lastWheelLight1_ = true;
    }
}
DcsBios::IntegerBuffer lightGearLBuffer(0x447a, 0x8000, 15, onLightGearLChange);




//CMDS
const byte cmdsModeKnbPins[6] = {41, 43, 45, 47, 49, 51};
DcsBios::SwitchMultiPos cmdsModeKnb("CMDS_MODE_KNB", cmdsModeKnbPins, 6);
const byte cmdsProgKnbPins[5] = {40, 42, 44, 46, 48};
DcsBios::SwitchMultiPos cmdsProgKnb("CMDS_PROG_KNB", cmdsProgKnbPins, 5);

DcsBios::Switch2Pos cmdsPwrSourcheSw("CMDS_PWR_SOURCHE_SW", 28);
DcsBios::Switch2Pos cmdsJmrSourcheSw("CMDS_JMR_SOURCHE_SW", 26);
DcsBios::Switch2Pos cmdsMwsSourcheSw("CMDS_MWS_SOURCHE_SW", 24);
DcsBios::Switch2Pos cmdsJettSw("CMDS_JETT_SW", 22);
DcsBios::Switch2Pos cmds01ExpCatSw("CMDS_01_EXP_CAT_SW", 30);
DcsBios::Switch2Pos cmds02ExpCatSw("CMDS_02_EXP_CAT_SW", 32);
DcsBios::Switch2Pos cmdsChExpCatSw("CMDS_CH_EXP_CAT_SW", 34);
DcsBios::Switch2Pos cmdsFlExpCatSw("CMDS_FL_EXP_CAT_SW", 36);


typedef DcsBios::PotentiometerEWMA<POLL_EVERY_TIME, 3000,3> McPitPot; //the 3000 is the hystorisis counts 
McPitPot hmcsIntKnb("HMCS_INT_KNB", A0);

DcsBios::Switch2Pos canopyHandle("CANOPY_HANDLE", 4);
DcsBios::Switch3Pos canopySw("CANOPY_SW", 6, 5);
DcsBios::Switch2Pos cmdsDispenseBtn("CMDS_DISPENSE_BTN", 7);


//Stuff to drive the LED!
int intensVal = 1;
LedControl lc=LedControl(52,53,50,1);//DIN,clk,cs

void onCmdsO1AmountChange(char* newValue) {
    lastcmds_[7] = newValue[2];
    lastcmds_[6] = newValue[3];
    lc.setChar(0,7,newValue[2],false);
    lc.setChar(0,6,newValue[3],false);
}
DcsBios::StringBuffer<4> cmdsO1AmountBuffer(0x45a0, onCmdsO1AmountChange);

void onCmdsO2AmountChange(char* newValue) {
    lastcmds_[5] = newValue[2];
    lastcmds_[4] = newValue[3];
    lc.setChar(0,5,newValue[2],false);
    lc.setChar(0,4,newValue[3],false);
}
DcsBios::StringBuffer<4> cmdsO2AmountBuffer(0x45a4, onCmdsO2AmountChange);

void onCmdsChAmountChange(char* newValue) {
    lastcmds_[3] = newValue[2];
    lastcmds_[2] = newValue[3];
    lc.setChar(0,3,newValue[2],false);
    lc.setChar(0,2,newValue[3],false);
}
DcsBios::StringBuffer<4> cmdsChAmountBuffer(0x45a8, onCmdsChAmountChange);

void onCmdsFlAmountChange(char* newValue) 
{
    lastcmds_[1] = newValue[2];
    lastcmds_[0] = newValue[3];
    lc.setChar(0,1,newValue[2],false);
    lc.setChar(0,0,newValue[3],false);
}
DcsBios::StringBuffer<4> cmdsFlAmountBuffer(0x45ac, onCmdsFlAmountChange);

//Restore the led after a power off
void resetCMDSLED(char* newValue)
{
    lc.setChar(0,7,newValue[7],false);
    lc.setChar(0,6,newValue[6],false);
    lc.setChar(0,5,newValue[5],false);
    lc.setChar(0,4,newValue[4],false);
    lc.setChar(0,3,newValue[3],false);
    lc.setChar(0,2,newValue[2],false);
    lc.setChar(0,1,newValue[1],false);
    lc.setChar(0,0,newValue[0],false);
}


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



int floodPwmPout_ = 3;
byte floodOutLevel_ = 0;
void onFloodConsolesBrtKnbChange(unsigned int newValue) 
{
      floodOutLevel_ = map(newValue, 0, 65535, 0, 255);
      analogWrite( floodPwmPout_, floodOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer floodConsolesBrtKnbBuffer(0x4414, 0xffff, 0, onFloodConsolesBrtKnbChange);




//LCD functions
void initLCD()
{
 
  lc.shutdown(0,false);
  lc.setIntensity(0,intensVal);
  lc.clearDisplay(0);
  delay(200);
  lc.setChar(0,7,'1',false);
  lc.setChar(0,6,'2',false);
  lc.setChar(0,5,'3',false);
  lc.setChar(0,4,'4',false);
  lc.setChar(0,3,'5',false);
  lc.setChar(0,2,'6',false);
  lc.setChar(0,1,'7',false);
  lc.setChar(0,0,'8',false);
  delay(200);
  lastcmds_[7]  = '1';
  lastcmds_[6]  = '2';
  lastcmds_[5]  = '3';
  lastcmds_[4]  = '4';
  lastcmds_[3]  = '5';
  lastcmds_[2]  = '6';
  lastcmds_[1]  = '7';
  lastcmds_[0]  = '8';
}


void PollAllControls()
{
    emergStroreJett.resetThisState();
    antiSkidSw.resetThisState();
    hookSw.resetThisState();
    gndJettEnableSw.resetThisState();
    brakeChanSw.resetThisState();
    storesConfigSw.resetThisState();
    hornSilenceBtn.resetThisState();
    landTaxiLightSw.resetThisState();
    dnLockBtn.resetThisState();
    gearHandle.resetThisState();    
    //RWR
    rwrActPwrBtn.resetThisState();
    rwrSearchBtn.resetThisState();
    rwrAltBtn.resetThisState();
    cmdsModeKnb.resetThisState();
    cmdsProgKnb.resetThisState();
    cmdsPwrSourcheSw.resetThisState();
    cmdsJmrSourcheSw.resetThisState();
    cmdsMwsSourcheSw.resetThisState();
    cmdsJettSw.resetThisState();
    cmds01ExpCatSw.resetThisState();
    cmds02ExpCatSw.resetThisState();
    cmdsChExpCatSw.resetThisState();
    cmdsFlExpCatSw.resetThisState();;
    hmcsIntKnb.resetThisState();
    //canopy
    canopyHandle.resetThisState();
    canopySw.resetThisState();
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

  //Setup LCD
  initLCD();
  
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights

  pinMode(floodPwmPout_, OUTPUT); //for the flood lights
  analogWrite( floodPwmPout_, floodOutLevel_); //pwm to drive the lights
  
  DcsBios::setup();  
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
