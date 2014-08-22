#include <EEPROM.h>

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int eeprom_x_max,eeprom_x_min;
  int eeprom_y_max,eeprom_y_min;
  int eeprom_x1_max,eeprom_x1_min;
  int eeprom_y1_max,eeprom_y1_min;
};


//TFT===================================
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define cs   5
#define dc   4
#define rst  -1  // you can also connect this to the Arduino reset

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

//JOY===================================
#include <Joypad.h>
#define JOYSTICK_DEAD_ZONE 25

int x_max,x_min,y_max,y_min,x1_max,x1_min,y1_max,y1_min;

int Joy_x,Joy_y,Joy1_x,Joy1_y;
boolean Joy_sw,Joy1_sw;

boolean but1,but2,but3,but4;

int sw_l,sw_r;

int s_lig,s_mic;

//UI=====================================
unsigned long timer1;
#define correct_timeout 3000

//DATA===================================
unsigned long time1=millis();

byte inBuf[16]={
  0xDC,0x05,0xDC,0x05,0xD0,0x07,0xEF,0x03,0xDC,0x05,0xDC,0x05,0xDC,0x05,0xDC,0x05
};

int outBuf[8]={
  1500,1500,2000,1007,1500,1500,1500,1500
};

int RCin[8],RCoutA[8],RCoutB[8];
int p;
uint16_t t,t1,t2;

uint16_t RCout[8];

uint16_t read16() {
  uint16_t r = (inBuf[p++]&0xFF);
  r+= (inBuf[p++]&0xFF)<<8;
  return r;
}

uint16_t write16(boolean a) {
  if(a)
  {
    t1=outBuf[p++]>>8;
    t2=outBuf[p-1]-(t1<<8);
    t=t1;
  }
  else
    t=t2;
  return t;
}

typedef  unsigned char byte;
byte getChecksum(byte length,byte cmd,byte mydata[]){
  //三个参数分别为： 数据长度  ，  指令代码  ，  实际数据数组
  byte checksum=0;
  checksum ^= (length&0xFF);
  checksum ^= (cmd&0xFF);
  for(int i=0;i<length;i++)
  {
    checksum ^= (mydata[i]&0xFF);
  }
  return checksum;
} 

void data_rx()
{
  //  s_struct_w((uint8_t*)&inBuf,16);
  p=0;
  for(int i=0;i<8;i++) {

    RCin[i]=read16();
    /*
    Serial.print("RC[");
     Serial.print(i+1);
     Serial.print("]:");
     
     Serial.print(inBuf[2*i],DEC);
     Serial.print(",");
     Serial.print(inBuf[2*i+1],DEC);
     
     Serial.print("---");
     Serial.println(RCin[i]);
     */
    //    delay(50);        // delay in between reads for stability
  }
}

void data_tx()
{
  p=0;
  for(int i=0;i<8;i++) {
    RCout[i]=outBuf[i];
    RCoutA[i]=write16(1);
    RCoutB[i]=write16(0);

    /*
    Serial.print("RC[");
     Serial.print(i+1);
     Serial.print("]:");
     
     Serial.print(RCout[i]);
     
     Serial.print("---");
     
     Serial.print(RCoutA[i],DEC);
     Serial.print(",");
     Serial.print(RCoutB[i],DEC);
     
     Serial.println("");
     */
    //    delay(50);        // delay in between reads for stability
  }
}

void data_send()
{
  data_tx();

  static byte buf_head[3];
  buf_head[0]=0x24;  
  buf_head[1]=0x4D;  
  buf_head[2]=0x3C;

  static byte buf_length[1];
  buf_length[0]=0x10;

  static byte buf_code[1];
  buf_code[0]=0xC8;

  static byte buf_data[16];
  buf_data[0]=RCoutB[0];
  buf_data[1]=RCoutA[0];
  buf_data[2]=RCoutB[1];
  buf_data[3]=RCoutA[1];
  buf_data[4]=RCoutB[2];
  buf_data[5]=RCoutA[2];
  buf_data[6]=RCoutB[3];
  buf_data[7]=RCoutA[3];
  buf_data[8]=RCoutB[4];
  buf_data[9]=RCoutA[4];
  buf_data[10]=RCoutB[5];
  buf_data[11]=RCoutA[5];
  buf_data[12]=RCoutB[6];
  buf_data[13]=RCoutA[6];
  buf_data[14]=RCoutB[7];
  buf_data[15]=RCoutA[7];

  byte check_sum=getChecksum(16,200,buf_data);

  static byte buf_body[1];
  buf_body[0]=check_sum;

  for(int a=0;a<3;a++)
    Serial.write(buf_head[a]);
  for(int a=0;a<1;a++)
    Serial.write(buf_length[a]);
  for(int a=0;a<1;a++)
    Serial.write(buf_code[a]);
  for(int a=0;a<16;a++)
    Serial.write(buf_data[a]);
  for(int a=0;a<1;a++)
    Serial.write(buf_body[a]);
}

void data_begin()
{
  Joy();

  if(sw_l>1500)
    Joy_y=1000;

  outBuf[0]=Joy1_x;
  outBuf[1]=Joy1_y;
  outBuf[2]=Joy_x;
  outBuf[3]=Joy_y;
  outBuf[4]=sw_l;
  outBuf[5]=sw_r;
}

//EEPROM================================
void eeprom_read()
{
  //EEPROM读取赋值
  config_type config_readback;
  EEPROM_read(0, config_readback);

  x_max=config_readback.eeprom_x_max;
  x_min=config_readback.eeprom_x_min;
  y_max=config_readback.eeprom_y_max;
  y_min=config_readback.eeprom_y_min;
  x1_max=config_readback.eeprom_x1_max;
  x1_min=config_readback.eeprom_x1_min;
  y1_max=config_readback.eeprom_y1_max;
  y1_min=config_readback.eeprom_y1_min;
}

void eeprom_write()
{
  // 定义结构变量config，并定义config的内容
  config_type config;
  config.eeprom_x_max = x_max;
  config.eeprom_x_min = x_min;
  config.eeprom_y_max = y_max;
  config.eeprom_y_min = y_min;
  config.eeprom_x1_max = x1_max;
  config.eeprom_x1_min = x1_min;
  config.eeprom_y1_max = y1_max;
  config.eeprom_y1_min = y1_min;

  // 变量config存储到EEPROM，地址0写入
  EEPROM_write(0, config); 	
}

//JOY===================================
int Joy_dead_zone(int _Joy_vol)
{
  int C=_Joy_vol;
  int Joy_vol=0;
  if(abs(C)>JOYSTICK_DEAD_ZONE)
  {
    if (C>0)
      Joy_vol = C-JOYSTICK_DEAD_ZONE;
    else
      Joy_vol = C+JOYSTICK_DEAD_ZONE;
  }
  return Joy_vol;
}

void Joy_correct()
{
  //  tft.fillScreen(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(4, 10);
  tft.print("Exit correct:");
  tft.setTextSize(0);
  tft.setCursor(4, 20);
  tft.print("--Press key B");
  //==============================================
  tft.setTextSize(0);
  tft.setTextColor(ST7735_BLACK);
  //----------------------------------------------
  int x_c=Joy_dead_zone(Joypad.readJoystickX());
  if(x_c>x_max)
  {
    x_max=x_c;

    tft.fillRect(30,  120+8, 30, 9, ST7735_WHITE);
  }
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(2, 120+9);
  tft.print("xMax:");
  tft.print(x_max);

  if(x_c<x_min)
  {
    x_min=x_c;

    tft.fillRect(30,  120-1, 30, 9, ST7735_WHITE);
  }
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(2, 120);
  tft.print("xMin:");
  tft.print(x_min);

  //----------------------------------------------
  int y_c=Joy_dead_zone(Joypad.readJoystickY());
  if(y_c>y_max)
  {
    y_max=y_c;

    tft.fillRect(30,  120+8+9+9, 30, 9, ST7735_WHITE);
  }
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(2, 120+9+9+9);
  tft.print("yMax:");
  tft.print(y_max);

  if(y_c<y_min)
  {
    y_min=y_c;

    tft.fillRect(30,  120+8+9, 30, 9, ST7735_WHITE);
  }
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(2, 120+9+9);
  tft.print("yMin:");
  tft.print(y_min);

  //----------------------------------------------
  int x1_c=Joy_dead_zone(Joypad.readJoystick1X());
  if(x1_c>x1_max)
  {
    x1_max=x1_c;
    tft.fillRect(98,  120+8, 30, 9, ST7735_WHITE); 
  }
  tft.setCursor(70, 120+9);
  tft.print("xMax:");
  tft.print(x1_max);

  if(x1_c<x1_min)
  {
    x1_min=x1_c;
    tft.fillRect(98,  120-1, 30, 9, ST7735_WHITE);
  }
  tft.setCursor(70, 120);
  tft.print("xMin:");
  tft.print(x1_min);

  //----------------------------------------------
  int y1_c=Joy_dead_zone(Joypad.readJoystick1Y());
  if(y1_c>y1_max)
  {
    y1_max=y1_c;
    tft.fillRect(98,  120+8+9+9, 30, 9, ST7735_WHITE);
  }
  tft.setCursor(70, 120+9+9+9);
  tft.print("yMax:");
  tft.print(y1_max);

  if(y1_c<y1_min)
  {
    y1_min=y1_c;
    tft.fillRect(98,  120+8+9, 30, 9, ST7735_WHITE);
  }
  tft.setCursor(70, 120+9+9);
  tft.print("yMin:");
  tft.print(y1_min);

  //-------------------------
  tft.drawRect(30,  50, 4, 50, ST7735_BLACK);
  tft.drawRect(7,  73, 50, 4, ST7735_BLACK);

  tft.drawRect(94,  50, 4, 50, ST7735_BLACK);
  tft.drawRect(71,  73, 50, 4, ST7735_BLACK);
}

void Joy()
{
  sw_l=map(Joypad.readButton(SWITCH_L),0,1,2000,1000);
  sw_r=map(Joypad.readButton(SWITCH_R),0,1,2000,1000);
  //------------------------------------
  s_lig=map(Joypad.readLightSensor(),0,1023,0,99);
  s_mic=map(Joypad.readMicrophone(),0,1023,0,99);
  //------------------------------------
  Joy_x=Joy_dead_zone(Joypad.readJoystickX());
  if(Joy_x<0)
    Joy_x=map(Joy_x, x_min,0, 2000, 1500);
  else
    Joy_x=map(Joy_x, 0, x_max, 1500, 1000);

  if(Joy_x<1000) Joy_x=1000;
  if(Joy_x>2000) Joy_x=2000;

  //------------------------------------
  Joy_y=Joy_dead_zone(Joypad.readJoystickY());
  if(Joy_y<0)
    Joy_y=map(Joy_y, y_min,0, 2000, 1500);
  else
    Joy_y=map(Joy_y, 0, y_max, 1500, 1000);

  if(Joy_y<1000) Joy_y=1000;
  if(Joy_y>2000) Joy_y=2000;

  //------------------------------------
  Joy_sw=Joypad.readButton(JOYSTICK_SW);

  //------------------------------------
  Joy1_x=Joy_dead_zone(Joypad.readJoystick1X());
  if(Joy1_x<0)
    Joy1_x=map(Joy1_x, x1_min,0, 2000, 1500);
  else
    Joy1_x=map(Joy1_x, 0, x1_max, 1500, 1000);

  if(Joy1_x<1000) Joy1_x=1000;
  if(Joy1_x>2000) Joy1_x=2000;

  //------------------------------------
  Joy1_y=Joy_dead_zone(Joypad.readJoystick1Y());
  if(Joy1_y<0)
    Joy1_y=map(Joy1_y, y1_min,0, 2000, 1500);
  else
    Joy1_y=map(Joy1_y, 0, y1_max, 1500, 1000);

  if(Joy1_y<1000) Joy1_y=1000;
  if(Joy1_y>2000) Joy1_y=2000;

  //------------------------------------
  Joy1_sw=Joypad.readButton(JOYSTICK1_SW);

  //------------------------------------
  but1=Joypad.readButton(SWITCH_1);
  but2=Joypad.readButton(SWITCH_2);
  but3=Joypad.readButton(SWITCH_3);
  but4=Joypad.readButton(SWITCH_4);
  //====================================
}

//TFT===================================
void TFT_init()
{
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  //  Serial.println("init");
  //  uint16_t time = millis();
  tft.fillScreen(ST7735_WHITE);
  //  time = millis() - time;
  //  Serial.println(time, DEC);
}

void TFT_begin()
{
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(4, 20);
  tft.print("Microduino");

  tft.setTextSize(1);
  tft.setCursor(4, 60);
  tft.print("Enter correct:");
  tft.setTextSize(0);
  tft.setCursor(4, 70);
  tft.print("--Press key A");

  tft.setCursor(4, 150); 
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  for(int a=0;a<((millis()-timer1)/300);a++)
  {
    tft.print("-");
  }
  //  delay(50);
}

//------------------
#define _Q_font_x 2
#define _Q_font_y 23

#define _Q_x 33
#define _Q_y 24
#define _W_x 93
#define _W_y 5

const int _C_x_S=_Q_x+1;
const int _C_x_M=_Q_x+((_W_x+1)/2);
const int _C_x_E=_Q_x+_W_x-1;

String NAME[8]={
  "THROT","ROLL","PITCH","YAW","AUX1","AUX2","AUX3","AUX4"};

void TFT_ready()
{
  tft.setTextSize(0);
  tft.setTextColor(ST7735_BLACK);

  for(int a=0;a<8;a++)
  {
    int _a=a*15;

    tft.setCursor(_Q_font_x, _Q_font_y+_a);
    tft.print(NAME[a]);
    //------------------------------------------
    tft.drawRect(_Q_x, _Q_y+_a, _W_x, _W_y, ST7735_BLACK);  
  }
}


void TFT_run()
{
  //------------------
  for(int a=0;a<8;a++)
  {
    int _a=a*15;

    //------------------------------------------
    int _C_x_A0,_C_x_B0,_C_x_A,_C_x_B,_C_x_A1,_C_x_B1;
    int _C_x;

    if(outBuf[a]<1500)
    {
      _C_x=map(outBuf[a],1000,1500,_C_x_S,_C_x_M);

      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x-_C_x_S;

      _C_x_A=_C_x;
      _C_x_B=_C_x_M-_C_x;

      _C_x_A1=_C_x_M;
      _C_x_B1=_C_x_E-_C_x_M;
    }
    else if(outBuf[a]>1500)
    {
      _C_x=map(outBuf[a],1500,2000,_C_x_M,_C_x_E);

      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x_M-_C_x_S;

      _C_x_A=_C_x_M;
      _C_x_B=_C_x-_C_x_M;

      _C_x_A1=_C_x;
      _C_x_B1=_C_x_E-_C_x;
    }
    else
    {
      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x_M-_C_x_S;

      _C_x_A=_C_x_M;
      _C_x_B=0;

      _C_x_A1=_C_x_M;
      _C_x_B1=_C_x_E-_C_x_M;
    }
    tft.fillRect(_C_x_A0,  _Q_y+_a+1, _C_x_B0, _W_y-2, ST7735_WHITE);
    tft.fillRect(_C_x_A,  _Q_y+_a+1, _C_x_B, _W_y-2, ST7735_BLUE);
    tft.fillRect(_C_x_A1,  _Q_y+_a+1, _C_x_B1, _W_y-2, ST7735_WHITE);

    tft.fillRect(_C_x_M,  _Q_y+_a-1, 1, _W_y+2, ST7735_RED);
    //--------------------------    
    //    tft.drawLine(80, 18, 80, 140, ST7735_RED);
  } 
}

void TFT_clear()
{
  tft.fillScreen(ST7735_WHITE);
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  Serial.println("===========hello===========");

  Serial.println("\n\r EEPROM READ...");
  eeprom_read();

  Serial.println("\n\r TFT INIT...");
  TFT_init();

  Serial.println("\n\r TFT BEGIN...");
  timer1=millis();
  unsigned long _TIME1=millis();
  while(millis()-_TIME1<correct_timeout)
  {
    TFT_begin();

    boolean correct=!Joypad.readButton(SWITCH_1);
    if(correct)
    {
      correct=false;

      Serial.println("\n\rCorrect IN...");
      x_min=0,x_max=0,y_min=0,y_max=0,x1_min=0,x1_max=0,y1_min=0,y1_max=0;

      Serial.println("\n\r TFT CLEAR...");
      TFT_clear();

      while(1)
      {
        Joy_correct();
        if(!Joypad.readButton(SWITCH_2))
          break;
      }
      Serial.println("\n\rCorrect OUT...");

      Serial.println("\n\r EEPROM WRITE...");
      eeprom_write();

      delay(1000);
    }
  }

  Serial.println("\n\r TFT CLEAR...");
  TFT_clear();
  Serial.println("\n\r TFT READY...");
  TFT_ready();
  Serial.println("===========start===========");
}

void loop() {
  uint16_t time = millis();

  data_begin();

  data_send();

  Serial.println("");

  TFT_run();

  time = millis() - time;
//  Serial.println(time, DEC);
}
