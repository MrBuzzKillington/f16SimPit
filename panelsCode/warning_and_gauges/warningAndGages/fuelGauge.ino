int fuelQty_ = 0;
int fuelQty100_ = 0;
int fuelQty1k_ = 0;
int fuelQty10k_ = 0;


//Control for the lcd on the fuel gauge
void initFuelLcd()
{
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
      for(;;); // Don't proceed, loop forever
    }
  
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    //display.display();
    //delay(2000); // Pause for 2 seconds
  
    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(3.8); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 7);;
    display.println(F("F16 v1"));
    display.display();
    delay (2000);
    // Clear the buffer
    display.clearDisplay();
    //setFuelDisplay(0) ;
}


void setFuelDisplay(int fuelQty) 
{
  display.clearDisplay();
  display.setTextSize(3.8); // Draw 2X-scale text
  //display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 7);
  char tmpStrng[10];
  sprintf(tmpStrng,"%05d",fuelQty);
  display.println(tmpStrng);
  //display.println(fuelQty);
  display.display();      // Show initial text
  //delay(100);
}



int normalizeFuelValue(unsigned int inputValue)
{
    int retValue = 0;
    
    if (inputValue > 0)
    {
        retValue = ceil(map(inputValue, 0, 65536, 0, 10)+1);
    }
    
    if (retValue > 9)
    {
      retValue = 0;
    }      

    return retValue;
}



//Convert fuel to int
void onFueltotalizer100Change(unsigned int newValue) 
{

    fuelQty100_ = normalizeFuelValue(newValue);
    
    int newFuelQty_ = fuelQty10k_*1e4 + fuelQty1k_*1e3 + fuelQty100_*1e2;
    //update was causing flicker in warning panel
    if (abs(newFuelQty_ - fuelQty_) > 99)
    { 
      fuelQty_ = newFuelQty_;
      setFuelDisplay(fuelQty_);
    }
}
DcsBios::IntegerBuffer fueltotalizer100Buffer(0x44f2, 0xffff, 0, onFueltotalizer100Change);


void onFueltotalizer1kChange(unsigned int newValue) 
{
    fuelQty1k_ = normalizeFuelValue(newValue);    
}
DcsBios::IntegerBuffer fueltotalizer1kBuffer(0x44f0, 0xffff, 0, onFueltotalizer1kChange);


void onFueltotalizer10kChange(unsigned int newValue) 
{
    fuelQty10k_ = normalizeFuelValue(newValue);
}
DcsBios::IntegerBuffer fueltotalizer10kBuffer(0x44ee, 0xffff, 0, onFueltotalizer10kChange);
