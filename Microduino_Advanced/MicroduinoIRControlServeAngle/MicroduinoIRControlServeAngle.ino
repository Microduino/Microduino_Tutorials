#include <IRremote.h>
#include <Servo.h>

int action;
int deg=0;
int t;

int RECV_PIN = 11;//定义红外接收器的引脚为11
IRrecv irrecv(RECV_PIN);
Servo servo;
decode_results results;

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

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外接收器
  servo.attach(9);//D9pin
  servo.write(0);
  t=0;
}

void loop() {

  deg=servo.read();
  if (irrecv.decode(&results))
  {
    action=deckey(results.value);
    Serial.println(results.value, HEX);//以16进制换行输出接收代码
    Serial.println(action);
    Serial.println();//为了便于观看输出结果增加一个空行

    switch(action)
    {
      case 1:
      servo.write(0);
      break;
      case 2:
      servo.write(20);
      break;
      case 3:
      servo.write(40);
      break;
      case 4:
      servo.write(60);
      break;
      case 5:
      servo.write(80);
      break;
      case 6:
      servo.write(100);
      break;
      case 7:
      servo.write(120);
      break;
      case 8:
      servo.write(140);
      break;
      case 9:
      servo.write(160);
      break;
    }
    delay(10);
    irrecv.resume();
  }

}