
#include <SPI.h>
#include <Wire.h>
#include "x168EasyDriver.h"


int calCurStp_ = 0;



//x168EasyDriver calStepper_(43,41); //FR
x168EasyDriver calStepper_(39,37); //AL




void setup() {
  // initialize serial:
  Serial.begin(9600);

  calCurStp_ = calStepper_.homeStepper(); //Home the stepper   
 //fuelALCurStp_ = fuelALStepper_.homeStepper(); //Home the stepper 

//   calCurStp_ = calStepper_.maxStepper(); //Home the stepper   
//  fuelALCurStp_ = fuelALStepper_.maxStepper(); //Home the stepper 

//  calCurStp_ = calStepper_.homeStepper(); //Home the stepper   
//  fuelALCurStp_ = fuelALStepper_.homeStepper(); //Home the stepper 


  //powerOnTest();
 
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    long red = Serial.parseInt();
   
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') 
    {


    //Plot raw
    //Use this to figure out what the full scal is, for volStps
    //calCurStp_ = calStepper_.gotoPosition(red);

    long val100 = 54500; //what is full scale from game
    long volStps = 1010;//full scal steps
    int newPos = map(red, 0, val100, 0, volStps);
    calCurStp_ = calStepper_.gotoPosition( newPos );  
     
     
      Serial.print("Value of : ");
      Serial.print(red);
     Serial.print(" is steps: ");
      Serial.print(newPos);
      Serial.println();
   
    }
  }
}
