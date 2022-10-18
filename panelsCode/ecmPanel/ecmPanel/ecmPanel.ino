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


//Button Press on/off
DcsBios::ActionButton ecm1BtnToggle("ECM_1_BTN", "TOGGLE", 14);
DcsBios::ActionButton ecm2BtnToggle("ECM_2_BTN", "TOGGLE", 15);
DcsBios::ActionButton ecm3BtnToggle("ECM_3_BTN", "TOGGLE", 16);
DcsBios::ActionButton ecm4BtnToggle("ECM_4_BTN", "TOGGLE", 17);
DcsBios::ActionButton ecm5BtnToggle("ECM_5_BTN", "TOGGLE", 18);
DcsBios::ActionButton ecm6BtnToggle("ECM_6_BTN", "TOGGLE", 19);
DcsBios::ActionButton ecmFrmBtnToggle("ECM_FRM_BTN", "TOGGLE", 20);
DcsBios::ActionButton ecmSplBtnToggle("ECM_SPL_BTN", "TOGGLE", 21);

DcsBios::Switch3Pos ecmPwSw("ECM_PW_SW", 8, 9);
DcsBios::Switch3Pos ecmXmitSw("ECM_XMIT_SW", 10, 11);
DcsBios::Switch2Pos ecmResetBtn("ECM_RESET_BTN", 12);
DcsBios::Switch2Pos ecmBitBtn("ECM_BIT_BTN", 13);



//Note all the A,F,T are tied together because im lazy
DcsBios::LED lightEcm1S(0x4480, 0x2000, 34);
DcsBios::LED lightEcm2S(0x448a, 0x0002, 32);
DcsBios::LED lightEcm3S(0x448a, 0x0020, 30);
DcsBios::LED lightEcm4S(0x448a, 0x0200, 28);
DcsBios::LED lightEcm5S(0x448a, 0x2000, 23);
DcsBios::LED lightEcmS(0x448c, 0x0002, 25);
DcsBios::LED lightEcmFrmS(0x448c, 0x0020, 27);
DcsBios::LED lightEcmSplS(0x448c, 0x0200, 29);
//Upper
DcsBios::LED lightEcm1A(0x4480, 0x4000, 26);
DcsBios::LED lightEcm1F(0x4480, 0x8000, 24);
DcsBios::LED lightEcm1T(0x448a, 0x0001, 22); //Upper Ts
//Lower
DcsBios::LED lightEcm5A(0x4480, 0x4000, 31);
DcsBios::LED lightEcm5F(0x4480, 0x8000, 33);
DcsBios::LED lightEcm5T(0x448a, 0x0001, 35); //Upper Ts

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
    ecmPwSw.resetThisState();
    ecmXmitSw.resetThisState();
    ecmResetBtn.resetThisState();
    ecmBitBtn.resetThisState();
}

void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1 && oldMaxPwrState == 0)
    {
      oldMaxPwrState = 1;
      PollAllControls();
   
    } 
    if (newValue == 0)
    {
      oldMaxPwrState = 0;
    }
}


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

 for (int i=22; i<36; i++)
 {
   pinMode(i, OUTPUT);  
 }

 //pinMode(4, OUTPUT);

 
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

 for (int i=22; i<36; i++)
 {
   digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(100);  
 }

 delay (100);
 
 for (int i=22; i<36; i++)
 {
   digitalWrite(i, LOW);   // turn the LED on (HIGH is the voltage level)
   delay(100);  
 }
 
  
 
}



  
