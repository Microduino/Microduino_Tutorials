#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>
#include "math.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	
// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//====set offset of x,y,z===========
#define Gx_offset -3.06 
#define Gy_offset 1.01
#define Gz_offset -0.88

#define LED0 8
#define LED1 9
#define LED2 10
#define LED3 11

//====================
MPU6050 accelgyro;
 
int16_t ax,ay,az;

int16_t gx,gy,gz;//Storing original data
float aax,aay,aaz,ggx,ggy,ggz;//The quantized data is stored
float Ax,Ay,Az;// g(9.8m/s^2)
float Gx,Gy,Gz;// °/s
int left,right,head,back;
int Mx,My,Mz,Nx,Ny,Nz;
 
float Angel_accX,Angel_accY,Angel_accZ;//Storing the calculated angle by acceleration
 
long LastTime,NowTime,TimeSpan;//Integral to the angular velocity

 
void setup()
{
  Wire.begin();
 
  Serial.begin(115200);
 
  Serial.println("Initializing I2C device.....");
  accelgyro.initialize();
 
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful":"MPU6050 connection failure");
 
  pinMode(LED0,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
}
 void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void u8g_box_frame() {
  u8g.drawLine(90,35,Mz,Nz);//z
  u8g.drawLine(90,35,My,Ny);//y
  u8g.drawLine(90,35,80,50);//x
}
void Rotate() {
if((-90<=Angel_accY)&&(Angel_accY<=0)&&(0<=Angel_accZ)&&(Angel_accZ<=90))
{
My=90+30*cos(Angel_accY);
Ny=35-30*sin(Angel_accY);
Mz=90+30*cos(Angel_accZ);
Nz=35-30*sin(Angel_accZ);
}
else if((-90<=Angel_accY)&&(Angel_accY<=0)&&(-90<=Angel_accZ)&&(Angel_accZ<=0))
{
My=90+30*cos(Angel_accY);
Ny=35-30*sin(Angel_accY);
Mz=90+30*cos(Angel_accZ);
Nz=35-30*sin(Angel_accZ);
}
else if((0<=Angel_accY)&&(Angel_accY<=90)&&(-90<=Angel_accZ)&&(Angel_accZ<=0))
{
My=90-30*cos(Angel_accY);
Ny=35-30*sin(Angel_accY);
Mz=90+30*cos(Angel_accZ);
Nz=35-30*sin(Angel_accZ);
}
else if((0<=Angel_accY)&&(Angel_accY<=90)&&(0<=Angel_accZ)&&(Angel_accZ<=90))
{
My=90+30*cos(Angel_accY);
Ny=35-30*sin(Angel_accY);
Mz=90-30*cos(Angel_accZ);
Nz=35-30*sin(Angel_accZ);
}
}
void draw(void) {
  u8g_prepare();
  u8g_box_frame();
  Rotate();
  u8g.setFont(u8g_font_6x10);
  u8g.setPrintPos(0, 16); 
  u8g.print("X:");
  u8g.print(Angel_accX);
  u8g.setPrintPos(0, 32); 
  u8g.print("Y:");
  u8g.print(Angel_accY);
  u8g.setPrintPos(0, 48); 
  u8g.print("Z:");
  u8g.print(Angel_accZ);
  
  u8g.setPrintPos(0, 0); 
  u8g.print(right);
  u8g.setPrintPos(8, 0); 
  u8g.print("--");
  u8g.print(left);
  u8g.setPrintPos(24, 0); 
   u8g.print("--");
  u8g.print(head);
  u8g.setPrintPos(40, 0); 
  u8g.print("--");
  u8g.print(back);
}
void loop()
{
  accelgyro.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);//Being three-axis acceleration and angular velocity

//======get Acceleration values of x,y,z
   Ax=ax/16384.00;
   Ay=ay/16384.00;
   Az=az/16384.00;
   //==========there are calculated angle between thethree axes of coordinate system by acceleration
   Angel_accX=atan(Ax/sqrt(Az*Az+Ay*Ay))*180/3.14;
   Angel_accY=atan(Ay/sqrt(Ax*Ax+Az*Az))*180/3.14;
   Angel_accZ=atan(Az/sqrt(Ax*Ax+Ay*Ay))*180/3.14;

  //==============================


  delay(10);//This is used to control the sampling rate
  //stop
  if((-5<=Angel_accX)&&(Angel_accX<=5)&&(-5<=Angel_accY)&&(Angel_accY<=5)&&(85<=Angel_accZ)&&(Angel_accZ<=90))
  {
right=0;left=0;head=0;back=0;
  }
  //right
  else if((-15<=Angel_accX)&&(Angel_accX<=15)&&(-90<=Angel_accY)&&(Angel_accY<=-5)&&(-5<=Angel_accZ)&&(Angel_accZ<=84))
  {
right=1;left=0;head=0;back=0;
  }
    //left
  else if((-15<=Angel_accX)&&(Angel_accX<=15)&&(-5<=Angel_accY)&&(Angel_accY<=84)&&(-5<=Angel_accZ)&&(Angel_accZ<=90))
  {
right=0;left=1;head=0;back=0;
  }
    //head
  else if((-5<=Angel_accX)&&(Angel_accX<=90)&&(-15<=Angel_accY)&&(Angel_accY<=15)&&(-5<=Angel_accZ)&&(Angel_accZ<=84))
  {
right=0;left=0;head=1;back=0;
  }
    //back
  else if((-90<=Angel_accX)&&(Angel_accX<=-5)&&(-15<=Angel_accY)&&(Angel_accY<=15)&&(-5<=Angel_accZ)&&(Angel_accZ<=84))
  {
right=0;left=0;head=0;back=1;
  }
  
  digitalWrite(LED0, right);
  digitalWrite(LED1, left);
  digitalWrite(LED2, head);
  digitalWrite(LED3, back);


    u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}

