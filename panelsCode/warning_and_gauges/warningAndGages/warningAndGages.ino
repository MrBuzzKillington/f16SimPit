/*
 Warning PAnel and gauges on right side
 */
#define DCSBIOS_IRQ_SERIAL
#include <Servo.h>
#include <Stepper.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "DcsBios.h"

//LCD stuff
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SSD1306_NO_SPLASH
#define OLED_RESET     -1//4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/*
 * Code to drive the warning panel led array
 */

//LED Matrix Driver
#define COL_1 22
#define COL_2 23
#define COL_3 24
#define COL_4 25
#define ROW_1 26
#define ROW_2 27
#define ROW_3 28
#define ROW_4 29
#define ROW_5 30
#define ROW_6 31
#define ROW_7 32
#define ROW_8 33
const byte ledCols_[] = {COL_1, COL_2, COL_3, COL_4, 0,0,0,0};
byte AllLedOff[] = {B00000000,B00000000,B00000000,B00000000};
byte AllLedOn[] = {B11111111,B11111111,B11111111,B11111111};
byte CurLedState_[] = {B00000000,B00000000,B00000000,B00000000};



/*
 *  Stepper motor 
 */
int x27MaxStep_= 600;
int x27MaxStpSpeed_ = 40;

int fuelFR_ = 0;
int fuelAL_ = 0;

Stepper fuelALStepper_(x27MaxStep_, 37, 39, 41, 43);
Stepper fuelFRStepper_(x27MaxStep_, 45, 47, 49, 51);



void onFuelFrChange(unsigned int newValue) 
{
    int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    if (newPos != fuelFR_)
    {
        fuelFR_ = StepTo( fuelFRStepper_ , fuelFR_, newPos);  
    }
}
DcsBios::IntegerBuffer fuelFrBuffer(0x44ec, 0xffff, 0, onFuelFrChange);


void onFuelAlChange(unsigned int newValue) {
    int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    if (newPos != fuelAL_)
    {
        fuelAL_ = StepTo( fuelALStepper_ , fuelAL_, newPos);  
    }
}
DcsBios::IntegerBuffer fuelAlBuffer(0x44ea, 0xffff, 0, onFuelAlChange);



//
//Sync the game with the buttons
//
void PollAllControls()
{
    //altFlapsSw.pollInputCurrent();  
}




//
//Read back the brightness for the LEDS
//
int consolePwmPout_ = 3;
byte consoleOutLevel_ = 0;

void onPriConsolesBrtKnbChange(unsigned int newValue) 
//
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 100);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);



int floodPwmPout_ = 2;
byte floodOutLevel_ = 0;
void onFloodConsolesBrtKnbChange(unsigned int newValue) 
{
      floodOutLevel_ = map(newValue, 0, 65535, 255, 0);
      analogWrite( floodPwmPout_, floodOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer floodConsolesBrtKnbBuffer(0x4414, 0xffff, 0, onFloodConsolesBrtKnbChange);





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
 
  setupWarning();
  setupSteppers();
  initFuelLcd();
  analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
  analogWrite( floodPwmPout_, floodOutLevel_); //pwm to drive the lights

  DcsBios::setup();
  }


void loop()
{
  DcsBios::loop();
  drawScreen(CurLedState_);
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
