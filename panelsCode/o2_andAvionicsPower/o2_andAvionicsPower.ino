/*
 Panel For the power and hut lighting
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

//Servo variables
Servo oxygenPressureServo;
int oxygenPressurePin = 6;
int oxygenPressureValue = 0;

//Sensor Panel

DcsBios::Switch3Pos engAntiIce("ENG_ANTI_ICE", 23, 25);
DcsBios::Switch3Pos iffAntSelSw("IFF_ANT_SEL_SW", 27, 29);
DcsBios::Switch3Pos uhfAntSelSw("UHF_ANT_SEL_SW", 31, 33);

DcsBios::Switch2Pos mmcPwrSw("MMC_PWR_SW", 35);
DcsBios::Switch2Pos stStaSw("ST_STA_SW", 37);
DcsBios::Switch2Pos mfdSw("MFD_SW", 39);
DcsBios::Switch2Pos ufcSw("UFC_SW", 41);

DcsBios::Switch2Pos mapSw("MAP_SW", 22);
DcsBios::Switch2Pos gpsSw("GPS_SW", 24);
DcsBios::Switch2Pos dlSw("DL_SW", 26);

const byte insKnbPins[7] = {28, 30, 32, 34, 36, 38, 40};
DcsBios::SwitchMultiPos insKnb("INS_KNB", insKnbPins, 7);
DcsBios::Switch3Pos midsLvtKnb("MIDS_LVT_KNB", 2, 3);


//Oxygen panel
const byte ky58FillKnbPins[8] = {14, 15, 16, 17, 18, 19, 20, 21};
DcsBios::SwitchMultiPos ky58FillKnb("KY58_FILL_KNB", ky58FillKnbPins, 8);

const byte ky58ModeKnbPins[4] = {42, 44, 46, 48};
DcsBios::SwitchMultiPos ky58ModeKnb("KY58_MODE_KNB", ky58ModeKnbPins, 4);

DcsBios::Switch3Pos ky58PwrKnb("KY58_PWR_KNB", 52, 50);

DcsBios::Potentiometer ky58VolKnb("KY58_VOL_KNB", A0);

DcsBios::ServoOutput flowIndicator(0x44f0, 5, 544, 2400);
//DcsBios::ServoOutput oxygenPressure(0x44ec, 6, 544, 2400);

DcsBios::Switch2Pos oxyDiluterLvr("OXY_DILUTER_LVR", 47);
DcsBios::Switch3Pos oxyEmergLvr("OXY_EMERG_LVR", 51, 49);
DcsBios::Switch3Pos oxySupplyLvr("OXY_SUPPLY_LVR", 45, 43 );


//consnent panel
//DcsBios::Switch3Pos plainCipherSw("PLAIN_CIPHER_SW", 10, 9);

//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 4;
int consolePwmPout2_ = 7;
int consolePwmPout3_ = 8;

byte consoleOutLevel_ = 0;
void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 100);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
    analogWrite( consolePwmPout2_, consoleOutLevel_); //pwm to drive the lights
    analogWrite( consolePwmPout3_, consoleOutLevel_); //pwm to drive the 
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);


//----------------------
//  Invert and adjust the pressure gauge
//----------------------
void onOxygenPressureChange(unsigned int newValue) 
{
    oxygenPressureValue = map(newValue, 0, 14000, 70, 1);   
    //oxygenPressureValue = map(newValue, 0, 65535, 2400, 544);   
    oxygenPressureServo.write( oxygenPressureValue );
}
DcsBios::IntegerBuffer oxygenPressureBuffer(0x44f6, 0xffff, 0, onOxygenPressureChange);


void PollAllControls()
{
      engAntiIce.resetThisState();
      iffAntSelSw.resetThisState();
      uhfAntSelSw.resetThisState();
      mmcPwrSw.resetThisState();
      stStaSw.resetThisState();
      mfdSw.resetThisState();
      ufcSw.resetThisState();
      mapSw.resetThisState();
      gpsSw.resetThisState();
      dlSw.resetThisState();
      insKnb.resetThisState();
      midsLvtKnb.resetThisState();
      ky58FillKnb.resetThisState();
      ky58ModeKnb.resetThisState();
      ky58PwrKnb.resetThisState();
      ky58VolKnb.resetThisState();
      oxyDiluterLvr.resetThisState();
      oxyEmergLvr.resetThisState();
      oxySupplyLvr.resetThisState();
}

void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1)
    {
      PollAllControls();
    } 
}
DcsBios::IntegerBuffer maxPwrSwBuffer(0x4424, 0x0008, 3, onMaxPwrSwChange);



// 
//  Main Functions
//
void setup() 
{
  oxygenPressureServo.attach( oxygenPressurePin ); //connect the servo object and pin
  //oxygenPressureServo.write( 70 );
  
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  pinMode(consolePwmPout2_, OUTPUT); //for the backlight
  pinMode(consolePwmPout3_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  analogWrite( consolePwmPout2_, consoleOutLevel_); //pwm to drive the lights
  analogWrite( consolePwmPout3_, consoleOutLevel_); //pwm to drive the lights
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
