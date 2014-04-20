#include <RGBLEDMatrix.h>


RGBLEDMatrix m = RGBLEDMatrix();

unsigned char datasmail[]={
  0x00,0x66,0x99,0x00,0x00,0x42,0x24,0x18};
unsigned char data[]={
  0x00,0x00,0x6C,0x92,0x92,0x92,0x92,0x00,//m//
  0x00,0x08,0x00,0x08,0x08,0x08,0x08,0x00,//i//
  0x00,0x00,0x38,0x04,0x04,0x04,0x38,0x00,//c//
  0x00,0x00,0x28,0x18,0x08,0x08,0x08,0x00,//r//
  0x00,0x00,0x18,0x24,0x24,0x24,0x18,0x00,//o//
  0x20,0x20,0x20,0x38,0x24,0x24,0x38,0x00,//d//
  0x00,0x00,0x24,0x24,0x24,0x24,0x78,0x00,//u//
  0x00,0x08,0x00,0x08,0x08,0x08,0x08,0x00,//i//
  0x00,0x00,0x18,0x24,0x24,0x24,0x24,0x00,//n//
  0x00,0x00,0x18,0x24,0x24,0x24,0x18,0x00,//o//
};

void setup(){
  m.init();
  m.clear();

}
void loop(){

  int ctr;

  m.clear();
  for(int row = 0; row < 8; ++row)
  {
    for(int pix = 0; pix < 8; ++pix)
    {
      m.set_led_rgb(row, pix, 0, 0, 255);
      delay(50);
    }
  }

  m.clear();
  m.set_led_rgb(2, 2, 255, 0, 0);//画点，（x,y,green,red,blue）
  delay(300);
  m.set_led_rgb(2, 5, 0, 255, 0);//画点，（x,y,green,red,blue）
  delay(300);
  m.set_led_rgb(5, 2, 0, 0, 255);//画点，（x,y,green,red,blue）
  delay(300);
  m.set_led_rgb(5, 5, 255, 255, 255);//画点，（x,y,green,red,blue）
  delay(300);

  m.clear();
  for(ctr=0; ctr < 2; ctr++) { 
    m.fader_hue();//整个屏幕变换颜色
  }

  m.clear();
  for(ctr=0; ctr < 1000; ctr++) { 
    m.color_wave(10);//从右往左每列刷屏，括号里面为颜色值
  }

  m.clear();
  for(ctr=0; ctr < 1; ctr++) { 
    m.matrix_test();//每行从上往下刷屏
  }
  m.clear();

  m.set_matrix_rgb(0,0,0); 

  for(ctr=0; ctr < 100; ctr++) { 
    m.matrix_heart(100);//画心，括号里面改变颜色值
  }

  m.clear();
  for(ctr=0; ctr < 4; ctr++) { 
    m.matrix_heart_2();//闪烁的心
  }
  m.clear();
  for(ctr=0; ctr < 10000; ctr++) { 
    m.random_leds();//全屏闪烁点
  }
  m.clear();
  m.explode(180,400);//从里向外画框（颜色，时间）
  m.clear();

  for(ctr=0; ctr < 500; ctr++) { 
    for(int i=0;i<8;i++)
    {
      m.matrix_image(i,datasmail[i], ctr);
    }
  }

  m.clear();
  move_lcd();

}

void move_lcd()
{
  for(int n=0;n<10;n++)
  {
    for(int k=0;k<6;k++)
    { 
      for(int j=0;j<8;j++)
      {
        if(n<9)
          m.matrix_image(j,data[j+8*n]>>k|data[j+8*(n+1)]<<6-k, 100*n);
        else
          m.matrix_image(j,data[j+8*n]>>k,100*n);
      }
      delay(200);
    }  
  }
}









