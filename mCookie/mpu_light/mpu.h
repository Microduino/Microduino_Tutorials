#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>//添加必须的库文件

//====一下三个定义了陀螺仪的偏差===========
#define Gx_offset -3.06 
#define Gy_offset 1.01
#define Gz_offset -0.88

//DEBUG-----------
#define Serial_DEBUG

//====================
MPU6050 accelgyro;

int16_t ax,ay,az;
int16_t gx,gy,gz;//存储原始数据
float Ax,Ay,Az;//单位 g(9.8m/s^2)

int Angel_accX,Angel_accY,Angel_accZ;//存储加速度计算出的角度
int Angel_accX_send,Angel_accY_send,Angel_accZ_send;//存储加速度计算出的角度

int Angel_accX_c,Angel_accY_c,Angel_accZ_c;

void getMPU()
{ 
  accelgyro.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);//获取三个轴的加速度和角速度

  //======一下三行是对加速度进行量化，得出单位为g的加速度值
  Ax=ax/16384.00;
  Ay=ay/16384.00;
  Az=az/16384.00;
  //==========以下三行是用加速度计算三个轴和水平面坐标系之间的夹角
  Angel_accX=atan(Ax/sqrt(Az*Az+Ay*Ay))*180/3.14;
  Angel_accY=atan(Ay/sqrt(Ax*Ax+Az*Az))*180/3.14;
  Angel_accZ=atan(Az/sqrt(Ax*Ax+Ay*Ay))*180/3.14;

  Angel_accX_send=map(abs(Angel_accX),0,90,0,140);
  Angel_accY_send=map(abs(Angel_accY),0,90,0,140);
  Angel_accZ_send=map(abs(Angel_accZ),0,90,0,140);

  //==============================
#ifdef Serial_DEBUG
  delay(5);//这个用来控制采样速度

  Serial.print("    X:");
  Serial.print(Angel_accX);
  Serial.print("    Y:");
  Serial.print(Angel_accY);
  Serial.print("    Z:");
  Serial.println(Angel_accZ);
#endif 
}
