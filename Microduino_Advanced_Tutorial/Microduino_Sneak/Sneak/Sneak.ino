#include <Wire.h> //I2C链接库
#include "Adafruit_LEDBackpack.h" //点阵驱动库
#include "Adafruit_GFX.h" //Arduino图形库
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); //RX,TX


Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix(); //设置I2C点阵型号

int speedS=6; //贪吃蛇的初始速度
int socre=0; //游戏起始得分
int socre_1=0;
int FX,FY; //事物的坐标
int SX,SY; //蛇头的坐标
int KEY,K; //串口取得按键码和比对后的按键码，这里后面有说明
char s[6]; //蛇身体坐标集合，一共3个点，6个元素

void setup() {
  Serial.begin(115200); 
  mySerial.begin(9600); 

  RANDOM(); //获取一个随机食物坐标
  s[1]=0;
  s[2]=2;
  s[3]=1;
  s[4]=2;
  s[5]=2;
  s[6]=2; //初始化蛇的身体坐标
  KEY=4; //初始化按键码，实际上就是行进方向，用1、2、3、4来代替4个正交方向

  matrix.begin(0x70); //设定IIC屏幕地址
  matrix.clear();  //屏幕清屏
  matrix.writeDisplay();  //屏幕显示
  matrix.setRotation(3);  //设置屏幕显示方向
  matrix.clear();

  matrix.setTextWrap(false);  //设置字体平滑模式
  matrix.setTextSize(1); //设置字体大小
  matrix.setTextColor(LED_GREEN); //设定LED颜色

  for (int8_t x=7; x>=-60; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Microduino");
    matrix.writeDisplay();
    delay(120);
  }

  matrix.writeDisplay();
  delay(1000);
  matrix.setTextColor(LED_RED); //设定LED颜色
  //循环绘制文字，获得滚动效果
  for (int8_t x=7; x>=-18; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("GO!");
    matrix.writeDisplay();
    delay(80);
  }

  //循环绘制蛇的身体
  matrix.clear(); 
  matrix.drawLine(0,2, 2,2, LED_GREEN);

  matrix.writeDisplay();
  delay(400);
  while(!Serial)
  {
  }
}
//随机食物获取函数 
void RANDOM()
{ 
  FX=random(0, 7); 
  FY=random(0, 7);
}
//重点说一下，这个是演示函数，实际上通过不断闪现食物来完成延迟，这样既可以做到精确延时，也可以让食物不断闪动，来让大家不断的看到~


void Delay(int timeF)
{
  for(int t=1;t<=timeF;t++)
  {
    matrix.drawPixel(FX, FY, LED_RED);
    matrix.writeDisplay();
    delay(50);
    matrix.drawPixel(FX, FY, LED_OFF);
    matrix.writeDisplay();
    delay(50);
  }
}

void gameover()
{
  matrix.setTextColor(LED_YELLOW);
  for (int8_t x=7; x>=-125; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("GAMEOVER YOUR SOCRE:");
    matrix.writeDisplay();
    delay(70);
  }
  //GameOver函数，显示游戏结束和得分
  matrix.setTextColor(LED_GREEN); //设定LED颜色
  for (int8_t x=7; x>=-39; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print(socre);
    matrix.writeDisplay();
    delay(70);
  }
  s[1]=0;
  s[2]=2;
  s[3]=1;
  s[4]=2;
  s[5]=2;
  s[6]=2;//重置蛇的坐标
  KEY=3;
  socre=0;
}
void loop() {

  //重点，这里是在比对是否方向和目前蛇的行进方向相反，相反就放弃按键
  if (mySerial.available() > 0){

    K=mySerial.read()-48;//获得串口发送的按键信息
    if (K>0)
      KEY=K;
  }
  else 
  {
    matrix.clear(); 
    if(KEY==1){
      s[1]=s[3];
      s[2]=s[4];
      s[3]=s[5];
      s[4]=s[6];
      s[6]=s[6]-1;
    }
    if(KEY==4){
      s[1]=s[3];
      s[2]=s[4];
      s[3]=s[5];
      s[4]=s[6];
      s[6]=s[6]+1;
    }
    if(KEY==2){
      s[1]=s[3];
      s[2]=s[4];
      s[3]=s[5];
      s[4]=s[6];
      s[5]=s[5]-1;
    }
    if(KEY==3){
      s[1]=s[3];
      s[2]=s[4];
      s[3]=s[5];
      s[4]=s[6];
      s[5]=s[5]+1;
    }
    //根据按键来移动蛇的身体
    for(int i=1;i<=6;i+=2)
    {
      matrix.drawPixel(s[i], s[i+1], LED_GREEN);
    }
    matrix.writeDisplay();
    Delay(speedS);
    SX=s[5];
    SY=s[6]; //获取蛇头的坐标
    if(SY<0 or SY>7 or SX<0 or SX>7){
      gameover();
    }
    //判断是否超出边框，超出就跳转到GameOver
    if(SY==FY&&SX==FX){
      socre=socre+1;
      RANDOM();
      Speed();
    }
    socre_1=socre;
  }
}

void Speed()
{
  //判断是否吃到了食物~吃到了就会加分和重新随机生成食物~
  if(socre_1!=socre&&socre%2==0&&socre>0)
    speedS=speedS-1; //根据得分加快蛇的速度
  if(speedS<=3)
    speedS=3;
}
