/*
 * 
 * BACKLIIGNTS on pin 2
 * 
 * 
 */


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
