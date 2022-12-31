/*
 EnginePanels
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>

#include "DcsBios.h"

// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//

unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;
bool lightsOut_ = false;

/*
 * 
 *   Define All the buttons here
 * 
 * 
 */

//IFF PAnel Panel
DcsBios::Switch3Pos iffEnableSw("IFF_ENABLE_SW", A3, A4);
DcsBios::Switch2Pos iffCIKnb("IFF_C_I_KNB", 14);
DcsBios::Switch3Pos iffM4CodeSw("IFF_M4_CODE_SW", 16, 15);
DcsBios::Switch3Pos iffM4ReplySw("IFF_M4_REPLY_SW", 18, 19);
const byte iffMasterKnbPins[5] = {3,4,5,6,7};
DcsBios::SwitchMultiPos iffMasterKnb("IFF_MASTER_KNB", iffMasterKnbPins, 5);
DcsBios::Switch2Pos iffM4MonitorSw("IFF_M4_MONITOR_SW", A2);



//Exterior lights
const byte antiCollLightKnbPins[8] = {38, 39, 40, 41, 42, 43, 44, 45};
DcsBios::SwitchMultiPos antiCollLightKnb("ANTI_COLL_LIGHT_KNB", antiCollLightKnbPins, 8);
DcsBios::Switch2Pos posFlashLightSw("POS_FLASH_LIGHT_SW", 51);
DcsBios::Switch3Pos posWingTailLightSw("POS_WING_TAIL_LIGHT_SW", 53, 52);
DcsBios::Switch3Pos posFuselageLightSw("POS_FUSELAGE_LIGHT_SW", 20, 21);
const byte masterLightSwPins[5] = {46, 47, 48, 49, 50};
DcsBios::SwitchMultiPos masterLightSw("MASTER_LIGHT_SW", masterLightSwPins, 5);
DcsBios::Potentiometer airRefuelLightKnb("AIR_REFUEL_LIGHT_KNB", A0);
DcsBios::Potentiometer formLightKnb("FORM_LIGHT_KNB", A1);

//Fuel
DcsBios::Switch2Pos fuelMasterCv("FUEL_MASTER_CV", 8);
DcsBios::Switch2Pos fuelMasterCv2("FUEL_MASTER_CV", 8,false,200);
DcsBios::Switch2Pos fuelMasterSw("FUEL_MASTER_SW", 8,false, 150);
DcsBios::Switch2Pos tankIntertingSw("TANK_INTERTING_SW", 9);
const byte engineFeedKnbPins[4] = {10, 11, 12, 13};
DcsBios::SwitchMultiPos engineFeedKnb("ENGINE_FEED_KNB", engineFeedKnbPins, 4);
DcsBios::Switch2Pos airRefuelSw("AIR_REFUEL_SW", 17);

//IFF switches are in seperate code file

//Canopy Ejection
DcsBios::Switch2Pos canopyJettThandle("CANOPY_JETT_THANDLE", A8,true);


//Function that gets call over and over
void onUpdateCounterChange(unsigned int newValue) 
{
    if (newValue%10 == 0) //read the IFF switches
    {
        checkIFFSwitches();     
    }
    
    lastUpdateCounter_ = newValue;
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);



void PollAllControls()
{
   
    iffEnableSw.resetThisState();
    iffCIKnb.resetThisState();
    iffM4CodeSw.resetThisState();
    iffM4ReplySw.resetThisState();
    
    iffMasterKnb.resetThisState();
    iffM4MonitorSw.resetThisState();
    
    antiCollLightKnb.resetThisState();
    posFlashLightSw.resetThisState();
    posWingTailLightSw.resetThisState();
    posFuselageLightSw.resetThisState();
    masterLightSw.resetThisState();
    airRefuelLightKnb.resetThisState();
    formLightKnb.resetThisState();
    
    //Fuel
    fuelMasterCv.resetThisState();
    fuelMasterSw.resetThisState();
    tankIntertingSw.resetThisState();
    engineFeedKnb.resetThisState();
    airRefuelSw.resetThisState();
  
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
 */

// 
//  Main Functions
//
void setup() 
{


  DcsBios::setup();
  initBackLight();
  initIFFSwitches();
  
}

void loop() {
  DcsBios::loop();
  //checkBackLightTimer();
  //This code checks eveyr 10 seconds to see if DCS is still locked and turns off the LEDS
 
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
