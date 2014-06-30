#include <RGBLEDMatrix.h>


RGBLEDMatrix m = RGBLEDMatrix();

void setup(){
  m.init();
  m.clear();

}
void loop(){
  int ctr;

  m.set_led_rgb(7, 2, 0, 0, 255);//画点，（x,y,red,green,blue）
  delay(3000);
  m.clear();
  for(int row = 0; row < 8; ++row)
  {
    for(int pix = 0; pix < 8; ++pix)
    {
      m.set_led_rgb(row, pix, 100, 100, 100);
      delay(50);
    }
  }
  for(ctr=0; ctr < 4; ctr++) { 
    m.fader();//白闪烁
  }


  for(ctr=0; ctr < 2; ctr++) { 
    m.fader_hue();//整个屏幕变换颜色
  }


  for(ctr=0; ctr < 1000; ctr++) { 
    m.color_wave(10);//从右往左每列刷屏，括号里面为颜色值
  }


  for(ctr=0; ctr < 100; ctr++) { 
    m.rainbow();//一列一种颜色
  }

  for(ctr=0; ctr < 10; ctr++) { 
    m.set_matrix_hue(0);//满屏，括号里面为颜色值
  }

  for(ctr=0; ctr < 1; ctr++) { 
    m.matrix_test();//每行从上往下刷屏
  }

  m.set_matrix_rgb(0,0,0);
  for(ctr=0; ctr < 250; ctr++) { 
    m.matrix_heart(255);//画心，括号里面改变颜色值
  }


  for(ctr=0; ctr < 4; ctr++) { 
    m.matrix_heart_2();//闪烁的心
  }


  for(ctr=0; ctr < 10000; ctr++) { 
    m.random_leds();//全屏闪烁点
  }

  m.smile_blink(100,8,400);//（颜色，循环长度，闪烁时间）

  m.explode(100,100);//从里向外画框（颜色，时间）
}















