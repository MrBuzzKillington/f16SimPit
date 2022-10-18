/*
 * 
 * Class for easyDriver stepper controller to push 
 *  an x168 stepper
 */

#include "x168EasyDriver.h"
#include "Arduino.h"


 

  
    x168EasyDriver::x168EasyDriver(int dirPin, int stepPin) 
    {
      dirPin_ = dirPin;
      stepPin_ = stepPin;
      init();
    }
    
    void x168EasyDriver::init() 
    {
      pinMode(dirPin_, OUTPUT);
      pinMode(stepPin_, OUTPUT);
      set_ccw();
    }


  /**
   **  set direction
   **/
    void x168EasyDriver::set_cw() 
    {
      digitalWrite(dirPin_, HIGH);
    }
    
    void x168EasyDriver::set_ccw()
    {
      digitalWrite(dirPin_, LOW);
    }


  /**
   **  go home
   **/
    int x168EasyDriver::homeStepper( )
    {
        set_ccw();

        for (int i = 0; i < numStepsMax_; i++)
        {
          digitalWrite(stepPin_, HIGH);
          digitalWrite(stepPin_, LOW);
          delay(1);
        }
        curPos_ = 0;
        return curPos_;
    }


   /**
   **  go to a specific position
   **/
    int x168EasyDriver::gotoPosition(int tgtPos)
    {
      
      int numStepToGo = abs(curPos_ - tgtPos);
      int dir = 1;
      if (numStepToGo < minStep_)
      {
          return curPos_;  
      }
      //Set direction
      if (curPos_ > tgtPos)
      {
        set_ccw();
        dir = -1;
      } else {
        set_cw();
        dir = 1;
      }

    
      for (int i = 0; i < numStepToGo; i++)
      {
         digitalWrite(stepPin_, HIGH);
         digitalWrite(stepPin_, LOW);
         delay(1);
      }

      curPos_ = curPos_ + (dir * numStepToGo);
      
      if (curPos_ < 0)
      {
        curPos_ = 0;
      }

      if (curPos_ > numStepsMax_)
      {
        curPos_ = numStepsMax_;
      }
      return curPos_;
    }

   
   
