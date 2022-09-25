

/*
 * 
 * Code for ejection handle
 * 
 */

 int currButState_ = 0;
 int lastButState_ = 0;
 
 void setupEject()
 {
    pinMode(ejectionPin_, INPUT_PULLUP);
    currButState_ = digitalRead(ejectionPin_);
    lastButState_ = currButState_;
 }

 //Function to call and check if the handle is pulled
 void checkEject()
 {
    currButState_ = digitalRead(ejectionPin_);
    if (currButState_ == lastButState_)
    {
      return; //Do nothing
    }
    if (currButState_ == HIGH) //debounce?
    {
      sendDcsBiosMessage("SEAT_EJECT_HANDLE", "1");
      delay(10);
      sendDcsBiosMessage("SEAT_EJECT_HANDLE", "1");
      delay(10);
      sendDcsBiosMessage("SEAT_EJECT_HANDLE", "1");
           delay(10);
      sendDcsBiosMessage("SEAT_EJECT_HANDLE", "1");
           delay(10);
      sendDcsBiosMessage("SEAT_EJECT_HANDLE", "1");
    }

    lastButState_ = currButState_;
 }
