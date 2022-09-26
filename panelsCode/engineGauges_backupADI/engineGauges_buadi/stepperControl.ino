

//Stepper functions
int StepForward(Stepper theStepper, int numSteps , int stepperPos )
{
  if (stepperPos <= x27MaxStep_)
  {
      theStepper.step(numSteps);
      stepperPos+=numSteps;
  }
  return stepperPos;
}

int HomeStepper(Stepper theStepper)
{
    theStepper.setSpeed(40);
    StepTo( theStepper, x27MaxStep_, 0 );
    theStepper.setSpeed(x27MaxStpSpeed_);
    return 0;
}

int StepTo( Stepper theStepper, int curPos, int destPos)
{
  int minSteps = 10; //about 5 deg
  int stepsToTake = destPos  - curPos; 
  if (abs(stepsToTake) >= minSteps)  //This is to keep it from moving alot for 1degree shifts
  {  
      theStepper.step(-1*stepsToTake);
      
      if (destPos == 0)  //Was this to make sure we got all the way home
      {
           theStepper.step(100);
           return 0;
      }
      return curPos +=stepsToTake;
  }
  return curPos;
  
  //if (destPos == 0)  //Was this to make sure we got all the way home
  //{
  //  theStepper.step(100);
  //}
  //curPos +=stepsToTake;    
  
}

void setupSteppers()
{
  //Configure the steppers
  OilStepper_.setSpeed(x27MaxStpSpeed_);
  oilPressureCurStp_ = HomeStepper(OilStepper_); //Home the stepper
  //Configure the steppers
  NozStepper_.setSpeed(x27MaxStpSpeed_);
  NozOpenCurStp_ = HomeStepper(NozStepper_); //Home the stepper

  RpmStepper_.setSpeed(x27MaxStpSpeed_);
  rpmCurStp_ = HomeStepper(RpmStepper_); //Home the stepper
  //Configure the steppers
  FtitStepper_.setSpeed(x27MaxStpSpeed_);
  FtitCurStp_ = HomeStepper(FtitStepper_); //Home the stepper

  
}
