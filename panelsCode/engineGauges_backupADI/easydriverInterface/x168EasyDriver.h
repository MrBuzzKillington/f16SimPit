#ifndef X168EASYDRIVER_H
#define X168EASYDRIVER_H

/*
 * 
 * Class for easyDriver stepper controller to push 
 *  an x168 stepper
 */



 
class x168EasyDriver 
{
  private:
    int stepDevisor = 2;//MS1/2   00=full, 10=half 01=1/4 11=1/8 (Default)
    int numStepsMax_ = 600*stepDevisor; 
    int stepPin_= 1;
    int dirPin_ = 2;
    int curPos_ = 0;
    int minStep_ = 10;

    
  public:

  
    x168EasyDriver(int dirPin, int stepPin);     
    void init() ;
    void set_cw();
    void set_ccw();

    int homeStepper( );
    int gotoPosition(int tgtPos);

    int getCurPos() { return curPos_; };
    
    int getStepRange() { return numStepsMax_; };
    void setStepRange(int maxStep) {numStepsMax_ = maxStep;};

    //Min step controls how far you have to request before it will actually move
    //This is to reduce the jitter movement from small step requests
    int getMinStep() { return minStep_;};
    void setMinStep(int minStep) { minStep_ = minStep;};
    

    
}; // don't forget the semicolon at the end of the class

#endif
