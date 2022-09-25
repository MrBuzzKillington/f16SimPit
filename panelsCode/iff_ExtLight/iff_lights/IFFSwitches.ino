/*
 * 
 *  The IFF knobs need some massaging to make work
 * 
 */
 
int iffM1S1Cur_ = 0; 
int iffM1S2Cur_ = 0;
int iffM3S1Cur_ = 0; 
int iffM3S2Cur_ = 0;


int iffM1S1DCS_ = 0; 
int iffM1S2DCS_ = 0;
int iffM3S1DCS_ = 0; 
int iffM3S2DCS_ = 0;

//Function to convert BCD into an int
int bcdToInt (int v3, int v2, int v1, int v0)
{
    int retVal = 0;
    if (v3 == LOW)
    {
      retVal = retVal + 8;
    }
    if (v2 == LOW)
    {
      retVal = retVal + 4;
    }
    if (v1 == LOW)
    {
      retVal = retVal + 2;
    }
    if (v0 == LOW)
    {
      retVal = retVal + 1;
    }
    return retVal;
}

  
    
void checkIFFSwitches()
{  
    processIFF_M1S2( );
    processIFF_M1S1( );
    processIFF_M3S2( );
    processIFF_M3S1( );  
}


//IF no change then no action
//if 8/9 reset back to known state
//if other, toggle until match
void processIFF_M1S2( )
{
      int lastIFFM1S2 = iffM1S2Cur_;
      iffM1S2Cur_ = bcdToInt(digitalRead(36),digitalRead(34),digitalRead(32),digitalRead(30));
      
      if (lastIFFM1S2 == iffM1S2Cur_)
      {
         return; //nothing to do
      }

      //if you chose 8, then we set back to zero
      if (iffM1S2Cur_ >= 7)
      {
         if (iffM1S2DCS_ == 7)
         {
            return;
         }
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            incSwitch("IFF_M1_SEL_2");
         }
         iffM1S2DCS_ = 7;
         return;        
      }
      if (iffM1S2Cur_ == 0)
      {
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            decSwitch("IFF_M1_SEL_2");
         }
         iffM1S2DCS_ = 0;
         return;        
      }
      
      while( iffM1S2DCS_ != iffM1S2Cur_)
      {
         if (iffM1S2DCS_ > iffM1S2Cur_)
         {
              decSwitch("IFF_M1_SEL_2");
              iffM1S2DCS_ = iffM1S2DCS_ - 1;
         } else {
              incSwitch("IFF_M1_SEL_2");
              iffM1S2DCS_ = iffM1S2DCS_ + 1;
         }
      }
}
void processIFF_M1S1( )
{
      int lastIFFM1S1 = iffM1S1Cur_;
      iffM1S1Cur_ = bcdToInt(digitalRead(28),digitalRead(26),digitalRead(24),digitalRead(22));
      
      if (lastIFFM1S1 == iffM1S1Cur_)
      {
         return; //nothing to do
      }

      //if you chose 8, then we set back to zero
      if (iffM1S1Cur_ >= 7)
      {
         if (iffM1S1DCS_ == 7)
         {
            return;
         }
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            incSwitch("IFF_M1_SEL_1");
         }
         iffM1S1DCS_ = 7;
         return;        
      }
      if (iffM1S1Cur_ == 0)
      {
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            decSwitch("IFF_M1_SEL_1");
         }
         iffM1S1DCS_ = 0;
         return;        
      }
      
      while( iffM1S1DCS_ != iffM1S1Cur_)
      {
         if (iffM1S1DCS_ > iffM1S1Cur_)
         {
              decSwitch("IFF_M1_SEL_1");
              iffM1S1DCS_ = iffM1S1DCS_ - 1;
         } else {
              incSwitch("IFF_M1_SEL_1");
              iffM1S1DCS_ = iffM1S1DCS_ + 1;
         }
      }
}
//MODE3
void processIFF_M3S2( )
{
      int lastIFFM3S2 = iffM3S2Cur_;
      iffM3S2Cur_ = bcdToInt(digitalRead(37),digitalRead(35),digitalRead(33),digitalRead(31));
      
      if (lastIFFM3S2 == iffM3S2Cur_)
      {
         return; //nothing to do
      }

      //if you chose 8, then we set back to zero
      if (iffM3S2Cur_ >= 7)
      {
         if (iffM3S2DCS_ == 7)
         {
            return;
         }
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            incSwitch("IFF_M3_SEL_2");
         }
         iffM3S2DCS_ = 7;
         return;        
      }
      if (iffM3S2Cur_ == 0)
      {
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            decSwitch("IFF_M3_SEL_2");
         }
         iffM3S2DCS_ = 0;
         return;        
      }
      
      while( iffM3S2DCS_ != iffM3S2Cur_)
      {
         if (iffM3S2DCS_ > iffM3S2Cur_)
         {
              decSwitch("IFF_M3_SEL_2");
              iffM3S2DCS_ = iffM3S2DCS_ - 1;
         } else {
              incSwitch("IFF_M3_SEL_2");
              iffM3S2DCS_ = iffM3S2DCS_ + 1;
         }
      }
}
void processIFF_M3S1( )
{
      int lastIFFM3S1 = iffM3S1Cur_;
      iffM3S1Cur_ = bcdToInt(digitalRead(29),digitalRead(27),digitalRead(25),digitalRead(23));
      
      if (lastIFFM3S1 == iffM3S1Cur_)
      {
         return; //nothing to do
      }

      //if you chose 8, then we set back to zero
      if (iffM3S1Cur_ >= 7)
      {
         if (iffM3S1DCS_ == 7)
         {
            return;
         }
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            incSwitch("IFF_M3_SEL_1");
         }
         iffM3S1DCS_ = 7;
         return;        
      }
      if (iffM3S1Cur_ == 0)
      {
         //force it to 7
         for (int i = 0; i < 10; i++)
         {
            decSwitch("IFF_M3_SEL_1");
         }
         iffM3S1DCS_ = 0;
         return;        
      }
      
      while( iffM3S1DCS_ != iffM3S1Cur_)
      {        
         if (iffM3S1DCS_ > iffM3S1Cur_)
         {
              decSwitch("IFF_M3_SEL_1");
              iffM3S1DCS_ = iffM3S1DCS_ - 1;
         } else {
              incSwitch("IFF_M3_SEL_1");
              iffM3S1DCS_ = iffM3S1DCS_ + 1;
         }
      }
}



void initIFFSwitches()
{
     //IFF pins
      pinMode(22, INPUT_PULLUP);
      pinMode(23, INPUT_PULLUP);
      pinMode(24, INPUT_PULLUP);
      pinMode(25, INPUT_PULLUP);
      pinMode(26, INPUT_PULLUP);
      pinMode(27, INPUT_PULLUP);
      pinMode(28, INPUT_PULLUP);
      pinMode(29, INPUT_PULLUP);
      pinMode(30, INPUT_PULLUP);
      pinMode(31, INPUT_PULLUP);
      pinMode(32, INPUT_PULLUP);
      pinMode(33, INPUT_PULLUP);
      pinMode(34, INPUT_PULLUP);
      pinMode(35, INPUT_PULLUP);
      pinMode(36, INPUT_PULLUP);
      pinMode(37, INPUT_PULLUP);
      //Get the current Switch state
      checkIFFSwitches();
      //force them all to 0;
      resetIFF();
}

void resetIFF()
{
  

    iffM1S2Cur_ = 10;
    iffM1S1Cur_ = 10;
    iffM3S2Cur_ = 10;
    iffM3S1Cur_ = 10;
    
    //force them all to 0
    for (int i = 0; i < 8; i++)
    {
        decSwitch("IFF_M3_SEL_2");
        decSwitch("IFF_M3_SEL_1");
        decSwitch("IFF_M1_SEL_2");
        decSwitch("IFF_M1_SEL_1");
    }
   iffM3S1DCS_ = 0;
   iffM3S2DCS_ = 0;
   iffM1S1DCS_ = 0;
   iffM1S2DCS_ = 0;

        
    processIFF_M1S2( );
    processIFF_M1S1( );
    processIFF_M3S2( );
    processIFF_M3S1( );  

  
}

int sleepTime_ = 150;
void decSwitch(char* swName)
{
      DcsBios::tryToSendDcsBiosMessage(swName, "0"); //down
      delay(sleepTime_);
      DcsBios::tryToSendDcsBiosMessage(swName, "1"); //middle
      delay(sleepTime_);
      
}

void incSwitch(char* swName)
{
      DcsBios::tryToSendDcsBiosMessage(swName, "2"); //up
      delay(sleepTime_);
      DcsBios::tryToSendDcsBiosMessage(swName, "1"); //middle
      delay(sleepTime_);
}
