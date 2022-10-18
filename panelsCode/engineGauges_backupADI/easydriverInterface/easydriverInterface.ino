/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#include "x168EasyDriver.h"



int curPosFTIT_ = 0;

x168EasyDriver stepperFTIT_(49, 51);


// the setup function runs once when you press reset or power the board
void setup() 
{
   
    curPosFTIT_ = stepperFTIT_.homeStepper();

}

// the loop function runs over and over again forever
void loop() 
{

  if (curPosFTIT_ > 0)
  {
      curPosFTIT_ = stepperFTIT_.gotoPosition( 0 );
  } else {
      curPosFTIT_= stepperFTIT_.gotoPosition( stepperFTIT_.getStepRange() );
  }

  delay(1000);                       // wait for a second
}
