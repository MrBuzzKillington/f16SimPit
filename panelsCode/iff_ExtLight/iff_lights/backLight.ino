/*
 * 
 * BACKLIIGNTS on pin 2
 * 
 * 
 */
unsigned long previousMillis_ = 0;        // will store last time LED was updated
int consolePwmPout_ = 2;  //This assumes its on Pin2!
byte consoleOutLevel_ = 0;

/*
 * 
 *   This controls backlight brightness
 * 
 */


void onPriConsolesBrtKnbChange(unsigned int newValue) 
{
    consoleOutLevel_ = map(newValue, 0, 65535, 0, 100);
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(0x440e, 0xffff, 0, onPriConsolesBrtKnbChange);

void initBackLight()
{
    pinMode(consolePwmPout_, OUTPUT); //for the backlight
    analogWrite( consolePwmPout_, consoleOutLevel_); //pwm to drive the lights
}
