

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
    //theStepper.setSpeed(10);
    StepTo( theStepper, x27MaxStep_, 0 );
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
  }

    
    return destPos;
  } else {
    return curPos; 
  }
  
  //if (destPos == 0)  //Was this to make sure we got all the way home
  //{
  //  theStepper.step(100);
  //}
  //curPos +=stepsToTake;    
  
}


void setupSteppers()
{
  //Configure the steppers
  calStepper_.setSpeed(x27MaxStpSpeed_);
  calCurStp_ = HomeStepper(calStepper_); //Home the stepper
 

  
}
