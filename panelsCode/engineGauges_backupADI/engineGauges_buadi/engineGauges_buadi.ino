/*
 EnginePanels
 */
#define DCSBIOS_IRQ_SERIAL
#include <SPI.h>
#include <Wire.h>
#include "x168EasyDriver.h"


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

x168EasyDriver OilStepper_(22,24);
x168EasyDriver NozStepper_(26,28);
x168EasyDriver RpmStepper_(30,32);
x168EasyDriver FtitStepper_(34,36);


void onEngineOilPressureChange(unsigned int newValue)
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 51310; //0, 51000, 0, 466
    long volStps = 1000;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != oilPressureCurStp_)
    {
        oilPressureCurStp_ = OilStepper_.gotoPosition( newPos );  
    }
}
DcsBios::IntegerBuffer engineOilPressureBuffer(0x44da, 0xffff, 0, onEngineOilPressureChange);


void onEngineNozzlePositionChange(unsigned int newValue) 
{
    long valNoz100 = 58588; //0, 51000, 0, 466
    long volNozStps = 932;
    int newPos = map(newValue, 0, valNoz100, 0, volNozStps);
    if (newPos != NozOpenCurStp_)
    {
        NozOpenCurStp_ = NozStepper_.gotoPosition( newPos );  
    }
}
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44dc, 0xffff, 0, onEngineNozzlePositionChange);


void onEngineTachometerChange(unsigned int newValue) 
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 52519; //0, 51000, 0, 466
    long volStps = 1000;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != rpmCurStp_)
    {
        rpmCurStp_ = RpmStepper_.gotoPosition( newPos);  
    }
}
DcsBios::IntegerBuffer engineTachometerBuffer(0x44de, 0xffff, 0, onEngineTachometerChange);


void onEngineFtitChange(unsigned int newValue) 
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 35400; //0, 51000, 0, 466
    long volStps = 650;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != FtitCurStp_)
    {
        FtitCurStp_ = FtitStepper_.gotoPosition( newPos );  
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
      oilPressureCurStp_ = OilStepper_.homeStepper(); //Home the stepper   
      NozOpenCurStp_ = NozStepper_.homeStepper(); //Home the stepper 
      rpmCurStp_ = RpmStepper_.homeStepper(); //Home the stepper     
      FtitCurStp_ = FtitStepper_.homeStepper(); //Home the stepper     
    } 
    if (newValue == 0)
    {
      oldMaxPwrState = 0;
    }
}


DcsBios::IntegerBuffer maxPwrSwBuffer(0x4424, 0x0008, 3, onMaxPwrSwChange);

DcsBios::LED lightEngFire(0x447a, 0x0008, 10);
DcsBios::LED lightEngine(0x447a, 0x0010, 9);
DcsBios::LED lightHydOilPress(0x447a, 0x0020, 13);
DcsBios::LED lightHydOilPressLower(0x447a, 0x0020, 12);
DcsBios::LED lightFlcs(0x447a, 0x0040, 6);
DcsBios::LED lightDbuOn(0x447a, 0x0080, 11);
DcsBios::LED lightToLdgConfig(0x447a, 0x0100, 4);
DcsBios::LED lightToLdgConfigLower(0x447a, 0x0100, 7);
DcsBios::LED lightOxyLow(0x447a, 0x0400, 8);
DcsBios::LED lightCanopy(0x447a, 0x0200, 5);

DcsBios::LED lightDisc(0x447c, 0x0010, 14);
DcsBios::LED lightArNws(0x447c, 0x0008, 15);
DcsBios::LED lightRdy(0x447c, 0x0004, 16);


DcsBios::RotaryEncoder saiPitchTrim("SAI_PITCH_TRIM", "-500", "+500", 19, 18);
DcsBios::ActionButton saiCageToggle("SAI_CAGE", "TOGGLE", 17);
//DcsBios::Switch2Pos saiCage("SAI_CAGE", 17);



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

  oilPressureCurStp_ = OilStepper_.homeStepper(); //Home the stepper   
  NozOpenCurStp_ = NozStepper_.homeStepper(); //Home the stepper 
  rpmCurStp_ = RpmStepper_.homeStepper(); //Home the stepper     
  FtitCurStp_ = FtitStepper_.homeStepper(); //Home the stepper  


   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
   pinMode(8, OUTPUT);
   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);
   pinMode(11, OUTPUT);
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
 
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

 digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);    
  digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);  
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);  
  digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);  
   digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
 digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
 digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
 digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
 digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 
 digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100); 

  // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(100);     


  oilPressureCurStp_ = OilStepper_.maxStepper( );
  NozOpenCurStp_ = NozStepper_.maxStepper(  );
  rpmCurStp_ = RpmStepper_.maxStepper( );
  FtitCurStp_ = FtitStepper_.maxStepper( );

  oilPressureCurStp_ = OilStepper_.homeStepper(); //Home the stepper   
  NozOpenCurStp_ = NozStepper_.homeStepper(); //Home the stepper 
  rpmCurStp_ = RpmStepper_.homeStepper(); //Home the stepper     
  FtitCurStp_ = FtitStepper_.homeStepper(); //Home the stepper  
 

}



  
