#include <AccelStepper.h>

int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 5; //digital pin 2
int motorStepPin = 6; //digital pin 3

const int ENPin =  4;      // the number of the LED pin

#define ANALOG_IN A0

String comdata = "";  //缓存字符串
String onedata = "";  //缓存字符串

int data,deg;

AccelStepper stepper(1, motorStepPin, motorDirPin); 

void setup(){
  Serial.begin(9600);
  pinMode(ENPin, OUTPUT);  
  digitalWrite(ENPin, LOW);
  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);

}

void loop(){
  while (Serial.available() > 0)  //判断串口是否有输入
  {
    comdata = "";                     //清空显示字符
    onedata += char(Serial.read()); //读取字符
    comdata += onedata;               //同上
    delay(2);                         //等待串口缓存
    for(int i = 0; i < comdata.length(); i++)
    {
      switch(comdata.length())
      {
      case 1:
        data=(comdata[i]-'0');
        break;
      case 2:
        data=(comdata[i-1]-'0')*10+(comdata[i]-'0');
        break;
      case 3:
        data=(comdata[i-2]-'0')*100+(comdata[i-1]-'0')*10+(comdata[i]-'0');
        break;
      }
    }
    deg=data/1.8*16;
  }
  onedata = "";  //缓存字符串清空
  stepper.moveTo(deg);
  stepper.run();
}




