
#include <Servo.h>
#include <Stepper.h>
#include <SPI.h>
#include <Wire.h>




int x27MaxStep_= 600; //315 degree
int x27MaxStpSpeed_ = 20;

int calCurStp_ = 0;

//Stepper OilStepper_(x27MaxStep_, 29, 27, 25, 23);
//Stepper NozStepper_(x27MaxStep_, 37, 35, 33, 31);
//Stepper RpmStepper_(x27MaxStep_, 28, 26, 24, 22);
//Stepper FtitStepper_(x27MaxStep_,36, 34, 32, 30);

Stepper calStepper_(x27MaxStep_, 36, 34, 32, 30);


void setup() {
  // initialize serial:
  Serial.begin(9600);

  setupSteppers();
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
    //calCurStp_ = StepTo( calStepper_, calCurStp_ , red);

    long val100 = 35400; //0, 51000, 0, 466
    long volStps = 325;//340;
    int newPos = map(red, 0, val100, 0, volStps);
     calCurStp_ = StepTo( calStepper_, calCurStp_ , newPos);
     
     
      Serial.print("Value of : ");
      Serial.print(red);
     Serial.print(" is steps: ");
      Serial.print(newPos);
      Serial.println();
   
    }
  }
}
