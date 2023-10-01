// Flight controller for Cougar
//
//updated 5/19/21 changed the type to joystick due to windows10 driver
//------------------------------------------------------------

#include "Joystick.h"

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,   //ID,Type,
  12, 0,                                                  //ButtonCount,hatswitchCount
  true, true, true, false, false, false,                 //xAxis,yAxis,zAxis,rxAxis,ryAxis,rzAxis
  true, true, false, true, false);                       //Rudder,throttle,accelerator,brake,steering


//This disables the idel and off button press because it interferes with windows joystick cal
int winCal_ = 0; //use this to enable windows joystick cal, then turn back off after


const int numReadings = 20;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

  int engineOff = 30;//160; //100
  int engineIdle = 60; //190
  int zeroThrotPos = 350;//60; //20
  int fullThrotPos = 650;//180; //160
  int calibrateEnable = 0;

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool autoSendMode = false;
int xAxisPin = A1;    //x potentiometer thumbHat
int yAxisPin = A0;    //y potentiometer thumbHat
int xyAxisBtnPin = 2;    //y potentiometer thumbHat
int xAxisValue = 0;
int yAxisValue = 0;

int dogfightPin1 = 3;
int dogfightPin2 = 5;
int speedBrkPin1 = 4;
int speedBrkPin2 = 6;
int rngBtnPin = 7;
int vhfPin1 = 8;
int vhfPin2 = 9;
int vhfPin3 = 10;
int vhfPin4 = 11;
int offPin = 13;
int idlePin = 12;
int antnPin = A2;
int rngPin = A3;
int throtlePin = A4;

int antennaValue = 0;
int rangeValue = 0;
int throttleValue = 0;

void checkBtn (int pin, int butonID)
{
   if (digitalRead(pin) != 0)
   {
      Joystick.releaseButton(butonID);
   } else {
      Joystick.pressButton(butonID);
   }
}


void checkEngineOFf(int encoderValue)
{
   if (encoderValue > engineOff)
   {
      Joystick.releaseButton(10);
   } else {
      Joystick.pressButton(10);
   }
}

void setup() 
{

  Serial.begin(9600); // open the serial port at 9600 bps:
  
  //XY thumb hat
  Joystick.setXAxisRange(300, 800);
  Joystick.setYAxisRange(100, 900);
  pinMode(xyAxisBtnPin, INPUT_PULLUP);
  pinMode(dogfightPin1, INPUT_PULLUP);
  pinMode(dogfightPin2, INPUT_PULLUP);
  pinMode(speedBrkPin1, INPUT_PULLUP);
  pinMode(speedBrkPin2, INPUT_PULLUP);
  pinMode(rngBtnPin, INPUT_PULLUP);
  pinMode(vhfPin1, INPUT_PULLUP);
  pinMode(vhfPin2, INPUT_PULLUP);
  pinMode(vhfPin3, INPUT_PULLUP);
  pinMode(vhfPin4, INPUT_PULLUP);
  pinMode(offPin, INPUT_PULLUP);
  pinMode(idlePin, INPUT_PULLUP);

  
  Joystick.setBrakeRange(0, 1023);
  Joystick.setZAxisRange(0, 1023);
  Joystick.setThrottleRange(0,1023); //Why is this 128,1023
 

  
  //Configure joystick mode
  if (autoSendMode)
  {
    Joystick.begin();
  }
  else
  {
    Joystick.begin(false);
  }
}

void loop()
{
  //READ analog for xy
   xAxisValue = analogRead(xAxisPin);
   yAxisValue = analogRead(yAxisPin);
   Joystick.setXAxis(xAxisValue);
   Joystick.setYAxis(yAxisValue);

   //other pots
   antennaValue =analogRead(antnPin);
   rangeValue = analogRead(rngPin);
   throttleValue = analogRead(throtlePin); 


   //if (throttleValue >= 512)
   //  {
   //     throttleValue = throttleValue - 1024;
   //  }
   //  throttleValue = throttleValue + zeroThrotPos; //value at most negative position


     //Map the throttle and limit to 0->1024
     //We have set the zero point properly now streatch to 1024
     if (calibrateEnable == 0)
     {
          throttleValue = map(throttleValue, zeroThrotPos, fullThrotPos, 0, 1023); 
     }

     if (throttleValue < 0)
     {
        throttleValue = 0;
     }
     if (throttleValue > 1023)
     {
        throttleValue = 1023;
     }




   //Average
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = throttleValue;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

   throttleValue = total / numReadings;  //smooth
   //unfortuantly the knob is mounted with the roll over in the middle
   //so make from -512 -> 512
   if (calibrateEnable == 1)
   {
       Serial.println(throttleValue); 
   }
   
   
   //throttleValue = (throttleValue);//-300) * 4.87;
   if (throttleValue < 0)
   {
     throttleValue = 0;
   }
   if (throttleValue > 1023)
   {
     throttleValue = 1023;
   }

   checkEngineOFf(throttleValue);


    
   Joystick.setBrake(antennaValue);
   Joystick.setZAxis(rangeValue);
   Joystick.setThrottle(throttleValue);
 
   //Each of teh buttons
   checkBtn(xyAxisBtnPin, 0);
   checkBtn(dogfightPin1, 1);
   checkBtn(dogfightPin2, 2);
   checkBtn(speedBrkPin1, 3);
   checkBtn(speedBrkPin2, 4);
   checkBtn(rngBtnPin, 5);
   checkBtn(vhfPin1, 6);
   checkBtn(vhfPin2, 7);
   checkBtn(vhfPin3, 8);
   checkBtn(vhfPin4, 9);
   //checkBtn(offPin, 10); ///This goes to the off switch button if we need it
//   checkBtn(idlePin, 11); //This was turned off already



    if (autoSendMode == false)
    {
      Joystick.sendState();
    }
      
   delay(10);
 
}
