#include "U8glib.h"
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { 
    '1','2','3','A'            }
  ,
  { 
    '4','5','6','B'            }
  ,
  { 
    '7','8','9', 'C'           }
  ,
  { 
    '*','0','#','D'           }
};
byte rowPins[ROWS] = {
  7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  3, 4, 5,  6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	

char customKey;
char num;
long timer;
int ylb=0;

void setup(){
  Serial.begin(115200);
}

void draw(void) {  
  u8g.setFont(u8g_font_6x13);
  u8g.setPrintPos(ylb, 64); 
  u8g.print("Welcome! www.microduino.cc");
  u8g.setFont(u8g_font_fub25);
  u8g.setPrintPos(64, 32); 
  u8g.print(num);
}
void loop(){
  if(millis()-timer>2)
  {
    timer=millis();
    ylb-=4;
  }
  customKey = customKeypad.getKey();
  if (customKey != NO_KEY){
    num=customKey;
  }

  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}

void volcdsetup(char* zi,int x,int y)
{
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_7x13);
    u8g.setPrintPos(x, y); 
    u8g.print(zi);
  } 
  while( u8g.nextPage() );
}











