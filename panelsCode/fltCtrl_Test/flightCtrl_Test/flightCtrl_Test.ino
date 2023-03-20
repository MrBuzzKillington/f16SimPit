/*
 EnginePanels
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


DcsBios::Switch2Pos altFlapsSw("ALT_FLAPS_SW", 38);
DcsBios::Switch2Pos digiBakSw("DIGI_BAK_SW", 40);
DcsBios::Switch2Pos manTfFlyupSw("MAN_TF_FLYUP_SW", 42,true);
DcsBios::Switch2Pos bitSw("BIT_SW", 44); //MagHeld!
DcsBios::Switch2Pos flcsResetSw("FLCS_RESET_SW", 46);
DcsBios::Switch2Pos leFlapsSw("LE_FLAPS_SW", 48);
DcsBios::Switch2Pos trimApDiscSw("TRIM_AP_DISC_SW", 41);

DcsBios::LED lightFlFail(0x447e, 0x0020, 50);
DcsBios::LED lightFlRun(0x447e, 0x0010, 52);

DcsBios::RotaryEncoder rollTrim("ROLL_TRIM", "-1000", "+1000", 47, 49);
DcsBios::RotaryEncoder pitchTrim("PITCH_TRIM", "-1000", "+1000", 51, 53);
DcsBios::RotaryEncoder yawTrim("YAW_TRIM", "-1000", "+1000", 43, 45);

DcsBios::ServoOutput pitchtrimind(0x44d2, 6, 2100, 744);
DcsBios::ServoOutput rolltrimind(0x44d0, 5, 2300, 800);


//Test panel
DcsBios::Switch2Pos fireOheatDetectBtn("FIRE_OHEAT_DETECT_BTN", 32);
DcsBios::Switch2Pos obogsSw("OBOGS_SW", 23); 
DcsBios::Switch2Pos malIndLtsTest("MAL_IND_LTS_TEST", 30);
DcsBios::Switch3Pos probeHeatSw("PROBE_HEAT_SW", 27, 25);
DcsBios::Switch2Pos epuGenTestSw("EPU_GEN_TEST_SW", 31); //momentary
DcsBios::Switch3Pos flcsPwrTestSw("FLCS_PWR_TEST_SW", 35, 33);

DcsBios::LED lightFlcsPwrA(0x447e, 0x0040, 22);
DcsBios::LED lightFlcsPwrB(0x447e, 0x0080, 24);
DcsBios::LED lightFlcsPwrC(0x447e, 0x0100, 28);
DcsBios::LED lightFlcsPwrD(0x447e, 0x0200, 26);
//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 2;
byte consoleOutLevel_ = 0;

void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 255);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);




void timeoutlightsOut( ) 
{
       consoleOutLevel_ = 0;
       analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights

      //turn off leds
       digitalWrite(50, LOW); //lightFlFailEpuGen
       digitalWrite(52, LOW); //lightFlRun
        digitalWrite(22, LOW); //FLC Power A
        digitalWrite(24, LOW); //B
        digitalWrite(26, LOW); //D
        digitalWrite(28, LOW); //C    
 }

void PollAllControls()
{
    altFlapsSw.resetThisState();
    digiBakSw.resetThisState();
    manTfFlyupSw.resetThisState();
    bitSw.resetThisState();
    flcsResetSw.resetThisState();
    leFlapsSw.resetThisState();
    trimApDiscSw.resetThisState();
    fireOheatDetectBtn.resetThisState();
    obogsSw.resetThisState();
    malIndLtsTest.resetThisState();
    probeHeatSw.resetThisState();
    epuGenTestSw.resetThisState();
    flcsPwrTestSw.resetThisState();
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
  initBackLight();
  powerOnTest();
  DcsBios::setup();

 }

void loop() {
  DcsBios::loop();
 
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


void powerOnTest()
{
        pinMode(50, OUTPUT);
        pinMode(52, OUTPUT);
        pinMode(22, OUTPUT);
        pinMode(24, OUTPUT);
        pinMode(26, OUTPUT);
        pinMode(28, OUTPUT);
        //walk a bits  
        int dlyTime = 100;
        digitalWrite(50, LOW); //lightFlFailEpuGen
        digitalWrite(52, LOW); //lightFlRun
        digitalWrite(22, LOW); //FLC Power A
        digitalWrite(24, LOW); //B
        digitalWrite(26, LOW); //D
        digitalWrite(28, LOW); //C  
        delay(dlyTime);
        digitalWrite(50, HIGH); //lightFlFailEpuGen
        delay(dlyTime);
        digitalWrite(52, HIGH); //lightFlRun
        delay(dlyTime);
        digitalWrite(22, HIGH); //FLC Power A
        delay(dlyTime);
        digitalWrite(24, HIGH); //B
        delay(dlyTime);
        digitalWrite(26, HIGH); //D
        delay(dlyTime);
        digitalWrite(28, HIGH); //C
        delay(2000);
        digitalWrite(50, LOW); //lightFlFailEpuGen
        delay(dlyTime);
        digitalWrite(52, LOW); //lightFlRun
        delay(dlyTime);
        digitalWrite(22, LOW); //FLC Power A
        delay(dlyTime);
        digitalWrite(24, LOW); //B
        delay(dlyTime);
        digitalWrite(26, LOW); //D
        delay(dlyTime);
        digitalWrite(28, LOW); //C

        analogWrite( consolePwmPout_, 0);
        for (int i=0; i<255; i+=32)
        {
          analogWrite( consolePwmPout_, i);
          delay(500);
        }
        delay(2000);
        analogWrite( consolePwmPout_, consoleOutLevel_);
}

void initBackLight()
{
    pinMode(consolePwmPout_, OUTPUT); //for the backlight
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
  
