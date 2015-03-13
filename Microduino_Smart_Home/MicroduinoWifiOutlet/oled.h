
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); //设置OLED型号
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar) 


void osd_setup(int _osd_setup,char* _osd_text)     //显示函数  
{
  u8g.firstPage();  
  do 
  {
    setFont_L;
    u8g.setPrintPos(4, 30); 
    u8g.print(_osd_text);
    u8g.drawFrame(0,48,128,14);
    if(_osd_setup)
      u8g.drawBox(0+2,48+2,map(_osd_setup,0,5,0,128-4),14-4);
  } 
  while( u8g.nextPage() );
}


//显示函数 
void volcd() {
  u8g.firstPage();
  do {
    setFont_L;
    u8g.setPrintPos(5, 10);
    u8g.print("IP: ");
    u8g.setPrintPos(25, 10);
    u8g.print(IPAdressShow);
    

    u8g.setPrintPos(5, 40);
    u8g.print("state: ");
    u8g.setPrintPos(50, 40);
    outletStateInfo=outletState?"ON":"OFF";
    u8g.print(outletStateInfo);

  }
  while( u8g.nextPage() );
} 
