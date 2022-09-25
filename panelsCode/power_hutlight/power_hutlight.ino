/*
 Panel For the power and hut lighting
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>

#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//




//Sensor Panel
DcsBios::Switch2Pos hdptSwL("HDPT_SW_L", 28);
DcsBios::Switch2Pos hdptSwR("HDPT_SW_R", 3);
DcsBios::Switch2Pos fcrPwrSw("FCR_PWR_SW", 4);
DcsBios::Switch3Pos rdrAltPwrSw("RDR_ALT_PWR_SW", 6, 5);
//HUD Control panel
DcsBios::Switch3Pos hudScalesSw("HUD_SCALES_SW", 8,7);//Wvah/vah
DcsBios::Switch3Pos hudFpMarkerSw("HUD_FP_MARKER_SW", 10, 9);
DcsBios::Switch3Pos hudDedDataSw("HUD_DED_DATA_SW", 12, 11);
DcsBios::Switch3Pos hudDepressRetSw("HUD_DEPRESS_RET_SW", 14, 13); 
DcsBios::Switch3Pos hudSpeedSw("HUD_SPEED_SW", 16, 15); //cas/tas/gnd
DcsBios::Switch3Pos hudAltSw("HUD_ALT_SW", 18, 17);
DcsBios::Switch3Pos hudBrtSw("HUD_BRT_SW", 20, 19);
DcsBios::Switch3Pos hudTestSw("HUD_TEST_SW", 24, 22);
DcsBios::Switch3Pos malIndLtsBrtSw("MAL_IND_LTS_BRT_SW", 26, 21);
//Brightness adjust
DcsBios::Potentiometer priConsolesBrtKnb("PRI_CONSOLES_BRT_KNB", A0);
DcsBios::Potentiometer priInstPnlBrtKnb("PRI_INST_PNL_BRT_KNB", A1);
DcsBios::Potentiometer priDataDisplayBrtKnb("PRI_DATA_DISPLAY_BRT_KNB", A2);
DcsBios::Potentiometer floodConsolesBrtKnb("FLOOD_CONSOLES_BRT_KNB", A3);
DcsBios::Potentiometer floodInstPnlBrtKnb("FLOOD_INST_PNL_BRT_KNB", A4);


//consnent panel
DcsBios::Switch3Pos plainCipherSw("PLAIN_CIPHER_SW", 51,53);

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


void PollAllControls()
{
 //   emergStroreJett.resetThisState();
      hdptSwL.resetThisState();
      hdptSwR.resetThisState();
      fcrPwrSw.resetThisState();
      rdrAltPwrSw.resetThisState();
      //HUD Control panel
      hudScalesSw.resetThisState();
      hudFpMarkerSw.resetThisState();
      hudDedDataSw.resetThisState();
      hudDepressRetSw.resetThisState(); 
      hudSpeedSw.resetThisState();
      hudAltSw.resetThisState();
      hudBrtSw.resetThisState();
      hudTestSw.resetThisState();
      malIndLtsBrtSw.resetThisState();
      //Brightness adjust
      priConsolesBrtKnb.resetThisState();
      priInstPnlBrtKnb.resetThisState();
      priDataDisplayBrtKnb.resetThisState();
      floodConsolesBrtKnb.resetThisState();
      floodInstPnlBrtKnb.resetThisState();
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
void setup() {
  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  DcsBios::setup();
 }

void loop() {
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
