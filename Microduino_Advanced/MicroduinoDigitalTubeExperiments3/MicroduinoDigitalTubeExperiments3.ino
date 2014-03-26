#include <IRremote.h>
#include <SevSeg.h>
SevSeg sevseg;
int RECV_PIN = 2;//定义红外接收器的引脚为2
IRrecv irrecv(RECV_PIN);
decode_results results;
int t;
int action;

int deckey(unsigned long t)
{
  switch(t){
    case 0xFD08F7://按钮1的编码
    return 1;
    break;
    case 0xFD8877://按钮2的编码
    return 2;
    break;
    case 0xFD48B7://按钮3的编码
    return 3;
    break;
    case 0xFD28D7://按钮4的编码
    return 4;
    break;
    case 0xFDA857://按钮5的编码
    return 5;
    break;
    case 0xFD6897://按钮6的编码
    return 6;
    break;
    case 0xFD18E7://按钮7的编码
    return 7;
    break;
    case 0xFD9867://按钮8的编码
    return 8;
    break;
    case 0xFD58A7://按钮9的编码
    return 9;
    break;
    default:
    return 0;
    break;
  }
} 

void setup() {
//定义需要使用的引脚
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
  irrecv.enableIRIn(); // 初始化红外接收器
  //sevseg.PrintOutput();
  //sevseg.NewNum(0, 4);
}

void loop() {
  //激活输出功能
  sevseg.PrintOutput();
  
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);//以16进制换行输出接收代码
    action=deckey(results.value);
    Serial.println(action);
    Serial.println();//为了便于观看输出结果增加一个空行
    
    sevseg.NewNum(action, 4);     //数码管函数输出数字，其中数值为小数点所在位置，我放在第四格所以效果就是不显示小数点
    
    irrecv.resume(); // 接收下一个值
  }


}