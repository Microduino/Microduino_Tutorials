
#include "SevSeg.h"
SevSeg sevseg;

void setup() {
  //定义需要使用的引脚
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
  pinMode(1, INPUT);    //按钮1引脚
  pinMode(2, INPUT);    //按钮2引脚
  pinMode(3, INPUT);    //按钮3引脚
  pinMode(4, INPUT);    //按钮4引脚

  sevseg.PrintOutput();
  sevseg.NewNum(0, 4); //初始值0

}

void loop() {
  //激活输出功能
  sevseg.PrintOutput();

  //循环读取这四个按钮的引脚
  for(int i=1; i<=4; i++) {
    if(digitalRead(i)==HIGH) {//如果有按钮按下
      while(1) {//显示按键编号
        sevseg.PrintOutput();
        sevseg.NewNum(i, 4);
      }
    }

  }




}


