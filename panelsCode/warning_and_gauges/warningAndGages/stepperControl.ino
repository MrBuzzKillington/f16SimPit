

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

int HomeStepper(Stepper theStepper, int stepperPos)
{
    //theStepper.setSpeed(10);
    theStepper.step(x27MaxStep_);
    theStepper.setSpeed(x27MaxStpSpeed_);
    //stepperPos=0;
    return stepperPos;
}

int StepTo( Stepper theStepper, int curPos, int destPos)
{

  int stepsToTake = destPos  - curPos;
  theStepper.step(-1*stepsToTake);
  if (destPos == 0)
  {
    theStepper.step(100);
  }
  return curPos +=stepsToTake;    
  
}


void setupSteppers()
{
  //Configure the steppers
  fuelFRStepper_.setSpeed(x27MaxStpSpeed_);
  fuelFR_ = HomeStepper(fuelFRStepper_, fuelFR_); //Home the stepper
  fuelALStepper_.setSpeed(x27MaxStpSpeed_);
  fuelAL_ = HomeStepper(fuelALStepper_, fuelAL_); //Home the stepper

StepTo( fuelFRStepper_, 0, x27MaxStep_);
StepTo( fuelALStepper_, 0, x27MaxStep_);
fuelFR_ = HomeStepper(fuelFRStepper_, fuelFR_); //Home the stepper
fuelAL_ = HomeStepper(fuelALStepper_, fuelAL_); //Home the stepper

  
}
