/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-OLEDM032-1
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with:
Works with Arduino 1.6.0 IDE  
Test OK : Arduino DUE,Arduino mega2560,Arduino UNO Board 
****************************************************/

/*
 Note:The module needs to be jumpered to an SPI interface. R19,R21 Short and R18,R20 Open  
 Unused signal pin Recommended to connect to GND 
  == Hardware connection ==
    OLED   =>    Arduino
  *1. GND    ->    GND
  *2. VCC    ->    3.3
  *4. SCL    ->    SCK
  *5. SDI    ->    MOSI
  *14. DC     ->    9
  *15. RES    ->    8  
  *16. CS     ->    10
*/


#include <SPI.h>
#include "er_oled.h"

//uint8_t oled_buf[OLED_Y_MAXPIXEL * OLED_X_MAXPIXEL/2];

void setup() {
  Serial.begin(9600);
  Serial.print("OLED Example\n");

  /* display an image of bitmap matrix */
  er_oled_begin();
  er_oled_clear();
  er_oled_bitmap_gray(PIC1);
  delay(3000);  

  er_oled_clear();
  er_oled_bitmap_gray(PIC2);
  delay(3000);
  
  er_oled_clear();
  er_oled_bitmap_mono(PIC3);
   delay(3000);

  er_oled_clear();
  er_oled_string(0, 0, "********************************",0);  
  er_oled_string(32, 16, "EastRising Technology",  0); 
  er_oled_string(40, 32, "www.buydisplay.com",  1); 
  er_oled_string(0, 48, "********************************",0); 
  uint8_t i;
  for(i=0;i<=48;i++)
  {
  command(0xa1); //start line
  data(i);
  delay(100);
  }
   for(i=48;i>0;i--)
  {
  command(0xa1); //start line
  data(i);
  delay(100);
  }
  
}

void loop() {

}

