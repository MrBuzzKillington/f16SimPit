/*
 * 
 *   ER-OLEDM032-1Y 
 *   This requires a library be install in you arduino libraries folder  U8G2
 * 
 * 
 */
//Create the oled and set the DC/res/cs pins
//



U8G2_SSD1322_NHD_256X64_1_4W_HW_SPI dedScreen(U8G2_R0, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5);
char* line1;
char* line2;
char* line3;
char* line4;
char* line5;

bool bufferChanged;



void onDedLine1Change(char* newValue) {
  line1 = newValue;
  bufferChanged = true;
}
DcsBios::StringBuffer<25> dedLine1Buffer(0x450a, onDedLine1Change);

void onDedLine2Change(char* newValue) {
  line2 = newValue;
  bufferChanged = true;
}
DcsBios::StringBuffer<25> dedLine2Buffer(0x4528, onDedLine2Change);

void onDedLine3Change(char* newValue) {
  line3 = newValue;
  bufferChanged = true;
}
DcsBios::StringBuffer<25> dedLine3Buffer(0x4546, onDedLine3Change);

void onDedLine4Change(char* newValue) {
  line4 = newValue;
  bufferChanged = true;
}
DcsBios::StringBuffer<25> dedLine4Buffer(0x4564, onDedLine4Change);

void onDedLine5Change(char* newValue) {
  line5 = (newValue);
  bufferChanged = true;
}
DcsBios::StringBuffer<25> dedLine5Buffer(0x4582, onDedLine5Change);





void initDED()
{
  dedScreen.begin();
  dedScreen.clearBuffer();
  dedScreen.setFont(DEDfont16px);
  dedScreen.firstPage();
  do {
      dedScreen.drawStr(0, 12, "***************************");
      dedScreen.drawStr(0, 25, "*       DCSBOIS F16       *"); 
      dedScreen.drawStr(0, 38, "*      DED + UFC V1.3     *"); //Column,Row,text,invert
      dedScreen.drawStr(0, 51, "*     BUZZKILLINGTON      *"); 
      dedScreen.drawStr(0, 64, "***************************"); 
  } while ( dedScreen.nextPage() );
  dedScreen.updateDisplay();
  delay(1000);
  bufferChanged = false;
}


 void updateDED()
 {
  if (bufferChanged == true)
  {
    bufferChanged = false;
    dedScreen.firstPage();
    do {
      dedScreen.drawStr(30, 12, line1);
      dedScreen.drawStr(30, 25, line2);
      dedScreen.drawStr(30, 38, line3);
      dedScreen.drawStr(30, 51, line4);
      dedScreen.drawStr(30, 64, line5);
    } while ( dedScreen.nextPage() );
    dedScreen.updateDisplay();
    
  }
 }
