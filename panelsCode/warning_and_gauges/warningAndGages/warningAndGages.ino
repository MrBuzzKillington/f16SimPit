/*
 Warning PAnel and gauges on right side
 */
#define DCSBIOS_IRQ_SERIAL
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "x168EasyDriver.h"


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


int oldMaxPwrState = 0;


/*
 *  Stepper motor 
 */
int x27MaxStep_= 600;
int x27MaxStpSpeed_ = 10;

int fuelFRCurStp_ = 0;
int fuelALCurStp_ = 0;


x168EasyDriver fuelFRStepper_(43,41);
x168EasyDriver fuelALStepper_(39,37);




void onFuelFrChange(unsigned int newValue) 
{
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 54500;
    long volStps = 1025;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != fuelFRCurStp_)
    {
        fuelFRCurStp_ = fuelFRStepper_.gotoPosition( newPos );  
    }
}
DcsBios::IntegerBuffer fuelFrBuffer(0x44ec, 0xffff, 0, onFuelFrChange);


void onFuelAlChange(unsigned int newValue) {
    //int newPos = map(newValue, 0, 65535, 0, x27MaxStep_);
    long val100 = 54500; //0, 51000, 0, 466
    long volStps = 1010;//340;
    int newPos = map(newValue, 0, val100, 0, volStps);
    if (newPos != fuelALCurStp_)
    {
        fuelALCurStp_ = fuelALStepper_.gotoPosition( newPos );  
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
  initFuelLcd();

  fuelFRCurStp_ = fuelFRStepper_.homeStepper(); //Home the stepper   
 fuelALCurStp_ = fuelALStepper_.homeStepper(); //Home the stepper 
  powerOnTest();
  
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
void onMaxPwrSwChange(unsigned int newValue)
{
    if (newValue == 1 && oldMaxPwrState == 0)
    {
      oldMaxPwrState = 1;
      DcsBios::resetAllStates();
      PollAllControls();

      //Reset the steppers     
      fuelFRCurStp_ = fuelFRStepper_.homeStepper(); //Home the stepper   
      fuelALCurStp_ = fuelALStepper_.homeStepper(); //Home the stepper 
  
    } 
    if (newValue == 0)
    {
      oldMaxPwrState = 0;
    }
}
DcsBios::IntegerBuffer maxPwrSwBuffer(0x4424, 0x0008, 3, onMaxPwrSwChange);


void powerOnTest()
{
  fuelFRCurStp_ = fuelFRStepper_.maxStepper(); //Home the stepper   
  fuelALCurStp_ = fuelALStepper_.maxStepper(); //Home the stepper 

  fuelFRCurStp_ = fuelFRStepper_.homeStepper(); //Home the stepper   
  fuelALCurStp_ = fuelALStepper_.homeStepper(); //Home the stepper 


}
