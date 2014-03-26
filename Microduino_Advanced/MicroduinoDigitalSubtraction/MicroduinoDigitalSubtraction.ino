
    #include "SevSeg.h"
    SevSeg sevseg;
    int i=5;
    
    void setup() {
    //定义需要使用的引脚
      sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
      pinMode(2, INPUT);    //D2用于加
      pinMode(3, INPUT);    //D3用于减
      
      sevseg.PrintOutput();
      sevseg.NewNum(i, 4); 
      
    }
     
    void loop() {
      //激活输出功能
      sevseg.PrintOutput();

      if(digitalRead(2)==HIGH) {//如果按钮2按下
        i++;
        sevseg.NewNum(i, 4);
        delay(200);        //延时50ms
      }
      else if(digitalRead(3)==HIGH) {//如果按钮3按下
        i--;
        sevseg.NewNum(i, 4);
        delay(200);        //延时50ms
      }

  }
