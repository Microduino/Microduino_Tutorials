#include "U8glib.h"
 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);// definition OLED connection
 
//show speed
void draw(float temp) {
  u8g.setFont(u8g_font_courB14);//font
  u8g.drawStr( 0, 20, "Tempture:");
  u8g.setPrintPos(0, 54);
  u8g.print(temp);
  u8g.print(" ( `c)");
}  
 
 
void setup()
{
}
 
 
void loop()
{
  float n = analogRead(A0);    //Read the voltage value of pin A0
  float vol = n * (5.0 / 1023.0*100);   //Storage temperature data, temperature data obtained from the voltage values ​​are translated
 
  u8g.firstPage();  
  do {
    draw(vol);
  } 
  while( u8g.nextPage() ); 
  delay(2);
}
