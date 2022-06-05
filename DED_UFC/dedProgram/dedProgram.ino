
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

#include <Keypad.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



//Some variables
unsigned long previousMillis_ = 0;        // will store last time LED was updated
unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;




//DCS Connections
//UFC 

//This code addes hysterisis to the pots to reduce commands
//typedef DcsBios::PotentiometerEWMA<POLL_EVERY_TIME, 3000,3> McPitPot; //the 3000 is the hystorisis counts 
//McPitPot icpHudBrtKnb("ICP_HUD_BRT_KNB", A0, true);
//McPitPot icpRasterContrKnb("ICP_RASTER_CONTR_KNB", A1);
//McPitPot icpRasterBrtKnb("ICP_RASTER_BRT_KNB", A2, true);

DcsBios::Potentiometer icpHudBrtKnb("ICP_HUD_BRT_KNB", A0, true);
DcsBios::Potentiometer icpRasterContrKnb("ICP_RASTER_CONTR_KNB", A1);
DcsBios::Potentiometer icpRasterBrtKnb("ICP_RASTER_BRT_KNB", A2, true);


//DcsBios::Potentiometer icpHudBrtKnb("ICP_HUD_BRT_KNB", A0, true);
//DcsBios::Potentiometer icpRasterContrKnb("ICP_RASTER_CONTR_KNB", A1);
//DcsBios::Potentiometer icpRasterBrtKnb("ICP_RASTER_BRT_KNB", A2, true);

//Disabled //DcsBios::Potentiometer icpReticleDepressKnb("ICP_RETICLE_DEPRESS_KNB", A3);   //POT IS BROKEN
//Switches on UFC
DcsBios::Switch3Pos icpDriftSw("ICP_DRIFT_SW", 12,10);
DcsBios::Switch3Pos icpFlirGainSw("ICP_FLIR_GAIN_SW", 11, 9);
//Dobber
DcsBios::Switch3Pos icpDataUpDnSw("ICP_DATA_UP_DN_SW", 43, 41);
DcsBios::Switch3Pos icpDataRtnSeqSw("ICP_DATA_RTN_SEQ_SW", 33, 8);
//ACES-II
int ejectionPin_ = 46;
DcsBios::Switch2Pos seatEjectSafe("SEAT_EJECT_SAFE", 48);


//Center console
DcsBios::Switch3Pos altModeLv("ALT_MODE_LV", 21,44);
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB","-3200", "+3200", 19, 20);
DcsBios::RotaryEncoder airspeedSetKnb("AIRSPEED_SET_KNB", "-3200", "+3200",42,40);
DcsBios::RotaryEncoder adiPitchTrim("ADI_PITCH_TRIM", "-3200", "+3200", 38,36); 

DcsBios::Switch2Pos ehsiMode("EHSI_MODE", 32);

DcsBios::RotaryEncoder ehsiCrsSetKnb("EHSI_CRS_SET_KNB","-3200", "+3200", 28, 26);
DcsBios::Switch2Pos ehsiCrsSet("EHSI_CRS_SET", 30);

DcsBios::RotaryEncoder ehsiHdgSetKnb("EHSI_HDG_SET_KNB", "-3200", "+3200", 22, 24);


DcsBios::Switch2Pos extFuelTransSw("EXT_FUEL_TRANS_SW", 34);
const byte fuelQtySelKnbPins[6] = {A9, A10, A11, A12, A13};
DcsBios::SwitchMultiPos fuelQtySelKnb("FUEL_QTY_SEL_KNB", fuelQtySelKnbPins, 5);
DcsBios::Switch2Pos fuelQtySelTKnb("FUEL_QTY_SEL_T_KNB",A8, true , 50 , "INC","DEC");

//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 13;
byte consoleOutLevel_ = 0;

void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 255);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights

}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);


//
// Controls
//
void PollAllControls()
{
    icpDriftSw.resetThisState();
    icpFlirGainSw.resetThisState();
    //Dobber
    icpDataUpDnSw.resetThisState();
    icpDataRtnSeqSw.resetThisState();
    seatEjectSafe.resetThisState();
    extFuelTransSw.resetThisState();
    fuelQtySelKnb.resetThisState();
}

void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1)
    {
      PollAllControls();
    } 
}
DcsBios::IntegerBuffer maxPwrSwBuffer(0x4422, 0x0004, 2, onMaxPwrSwChange);


/*
 * 
 * Main Functions
 * 
 */

void setup()
{
  testBacklight();
  initDED();
  
  DcsBios::setup();
  //DcsBios::resetAllStates();
  setupEject();
}

void loop() 
{
  updateDED();
  processMatrixKeys();
 checkEject();
  DcsBios::loop();
}


void testBacklight()
{
    for (int j=0;j<3;j++)
    {
      for (int i=0;i<255;i+=10)
      {
          analogWrite( consolePwmPout_, i ); //pwm to drive the lights
          delay(25);  
      }
      for (int i=255;i>=0;i-=10)
      {
          analogWrite( consolePwmPout_, i ); //pwm to drive the lights
          delay(25);  
      }
    }
    analogWrite( consolePwmPout_, 0 ); //pwm to drive the lights
}




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
