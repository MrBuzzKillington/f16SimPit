/*
 EnginePanels
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>
#include <Stepper.h>
#include <SPI.h>
#include <Wire.h>


#include "DcsBios.h"



// PUT commands from library
//file:///C:/Users/chris/Saved%20Games/DCS/Scripts/DCS-BIOS/doc/control-reference.html
//

unsigned long previousMillis_ = 0;        // will store last time LED was updated
unsigned int lastUpdateCounter_ = 0;
unsigned int priorUpdateCounter_ = 0;

int oldMaxPwrState = 0;


/*
 *  Stepper motor 
 */
int x27MaxStep_= 600; //315 degree
int x27MaxStpSpeed_ = 10;

int oilPressureCurStp_ = 0;
int NozOpenCurStp_ = 0;
int rpmCurStp_ = 0;
int FtitCurStp_ = 0;

Stepper OilStepper_(x27MaxStep_, 29, 27, 25, 23);
Stepper NozStepper_(x27MaxStep_, 37, 35, 33, 31);
Stepper RpmStepper_(x27MaxStep_, 28, 26, 24, 22);
Stepper FtitStepper_(x27MaxStep_,36, 34, 32, 30);


void onEngineOilPressureChange(unsigned int newValue)
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 51310; //0, 51000, 0, 466
    long volStps = 500;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != oilPressureCurStp_)
    {
        oilPressureCurStp_ = StepTo( OilStepper_ , oilPressureCurStp_, newPos);  
    }
}
DcsBios::IntegerBuffer engineOilPressureBuffer(0x44da, 0xffff, 0, onEngineOilPressureChange);


void onEngineNozzlePositionChange(unsigned int newValue) 
{
    long valNoz100 = 58588; //0, 51000, 0, 466
    long volNozStps = 466;
    int newPos = map(newValue, 0, valNoz100, 0, volNozStps);
    if (newPos != NozOpenCurStp_)
    {
        NozOpenCurStp_ = StepTo( NozStepper_ , NozOpenCurStp_, newPos);  
    }
}
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44dc, 0xffff, 0, onEngineNozzlePositionChange);


void onEngineTachometerChange(unsigned int newValue) 
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 52519; //0, 51000, 0, 466
    long volStps = 500;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != rpmCurStp_)
    {
        rpmCurStp_ = StepTo( RpmStepper_ , rpmCurStp_, newPos);  
    }
}
DcsBios::IntegerBuffer engineTachometerBuffer(0x44de, 0xffff, 0, onEngineTachometerChange);


void onEngineFtitChange(unsigned int newValue) 
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 35400; //0, 51000, 0, 466
    long volStps = 325;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != FtitCurStp_)
    {
        FtitCurStp_ = StepTo( FtitStepper_ , FtitCurStp_, newPos);  
    }
}
DcsBios::IntegerBuffer engineFtitBuffer(0x44e0, 0xffff, 0, onEngineFtitChange);


int ledState_ = 0;

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




void PollAllControls()
{
 //   altFlapsSw.resetThisState();
}

void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1 && oldMaxPwrState == 0)
    {
      oldMaxPwrState = 1;
      PollAllControls();

      //Reset the steppers
      oilPressureCurStp_ = HomeStepper(OilStepper_); //Home the stepper   
      NozOpenCurStp_ = HomeStepper(NozStepper_); //Home the stepper 
      rpmCurStp_ = HomeStepper(RpmStepper_); //Home the stepper     
      FtitCurStp_ = HomeStepper(FtitStepper_); //Home the stepper     
    } 
    if (newValue == 0)
    {
      oldMaxPwrState = 0;
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

  setupSteppers();
 
  powerOnTest();

  pinMode(consolePwmPout_, OUTPUT); //for the backlight
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  DcsBios::setup();

 }

void loop()
{
  DcsBios::loop();
}



void powerOnTest()
{
//    //HomeStepper(OilStepper_); //Home the stepper
    StepTo( OilStepper_, 0 , x27MaxStep_);
    oilPressureCurStp_ = HomeStepper(OilStepper_); //Home the stepper  



 
    //nozzle
    //HomeStepper(NozStepper_); //Home the stepper
    StepTo( NozStepper_, 0 , x27MaxStep_);
    NozOpenCurStp_ = HomeStepper(NozStepper_); //Home the stepper 
 
//    //RPM
//    //HomeStepper(RpmStepper_); //Home the stepper
    StepTo( RpmStepper_, 0 , x27MaxStep_);
    rpmCurStp_ = HomeStepper(RpmStepper_); //Home the stepper
//    
//    //FTIT
    //HomeStepper(FtitStepper_); //Home the stepper
    StepTo( FtitStepper_, 0 , x27MaxStep_);
    FtitCurStp_ = HomeStepper(FtitStepper_); //Home the stepper



//    delay(500);
//    oilPressureCurStp_ = StepTo( OilStepper_, oilPressureCurStp_ , map(35026, 0, 65535, 0, x27MaxStep_)) ;//should be 55%
//    delay(500);
//    oilPressureCurStp_ = StepTo( OilStepper_, oilPressureCurStp_ , map(16284, 0, 65535, 0, x27MaxStep_)) ;//should be 35026 25%
//    delay(500);
//    oilPressureCurStp_ = StepTo( OilStepper_, oilPressureCurStp_ , map(0, 0, 65535, 0, x27MaxStep_)) ; 


//    long maxNozScale = 65535;
//    delay(500);
//    NozOpenCurStp_ = StepTo( NozStepper_, NozOpenCurStp_ , map(51000, 0, 51000, 0, 466)) ;//should be 100%
//    delay(500);
//    NozOpenCurStp_ = StepTo( NozStepper_, NozOpenCurStp_ , map(20000, 0, maxNozScale, 0, x27MaxStep_)) ;//should be 22731 40%
//    delay(500);
//    NozOpenCurStp_ = StepTo( NozStepper_, NozOpenCurStp_ , map(0, 0, maxNozScale, 0, x27MaxStep_)) ; 

}



  
