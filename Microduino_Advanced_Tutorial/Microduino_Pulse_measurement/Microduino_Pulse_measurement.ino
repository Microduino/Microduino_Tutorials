#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	

int pulsePin = A0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 5;                // pin to blink led at each beat
int fadePin = 6;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

int data,num;

long timer,timey,time1,time2,time3;  //定义变量为浮点数

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

//阴码、逐行、逆向
static unsigned char u8g_logo_bits1[] U8G_PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x3C,
  0xFC,0x7E,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,
  0xFE,0xFF,0xFC,0x7F,0xF8,0x3F,0xF0,0x1F,
  0xE0,0x0F,0xC0,0x07,0x80,0x03,0x00,0x01//未命名文件",0//
};
static unsigned char u8g_logo_bits2[] U8G_PROGMEM = {
  0xFC,0x3F,0x02,0x41,0x05,0xA1,0x01,0x80,
  0x01,0x80,0x81,0x80,0x81,0x80,0x87,0xE0,
  0x81,0x80,0x01,0x81,0x01,0x82,0x01,0x84,
  0x01,0x88,0x05,0xA1,0x02,0x41,0xFC,0x3F,//未命名文件0//
};

void setup(){
  //u8g.setRot180();
  timer=millis();
  timey=millis();
  time1=millis();
  time2=millis();
  time3=millis();
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  pinMode(pulsePin,  INPUT_PULLUP);
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
  // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
  // AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //analogReference(EXTERNAL);   
}

void draw(void) { 
  
  u8g.drawXBMP( 90, 0, 16, 16, u8g_logo_bits1);
  u8g.drawXBMP( 90, 20, 16, 16, u8g_logo_bits2);

  u8g.setFont(u8g_font_fub20);
  u8g.setPrintPos(32, 64); 
  u8g.print(num); 
  u8g.setFont(u8g_font_9x18B);
  u8g.print("N/min");
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(0, 16); 
  u8g.print("Heat:");
  u8g.setFont(u8g_font_9x18B);
  u8g.print(data);
  u8g.setFont(u8g_font_7x14);
  u8g.print("N");
  u8g.setPrintPos(0, 34);  
  u8g.print("Time:");
  u8g.setFont(u8g_font_9x18B);
  u8g.print(timey);
  u8g.setFont(u8g_font_7x14);
  u8g.print("S");
}

void loop(){

  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat

    fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse


    QS = false;                      // reset the Quantified Self flag for next time    
  }

  ledFadeToBeat();

  if(fadeRate>230)   
    data++;
  if(data>0)
  {  

    if(millis()-timer>1000)
    {
      timer=millis();
      timey++;
    }
    if(timey>10)
      num=60*(data)/timey;
  }
  if(fadeRate==0)
  {
    if(millis()-time2>1000)
    {
      time2=millis();
      time3++;
    }
  }
  else
    time3=0;

  if(time3>1||timey>60)
  {
    data=0;
    num=0;
    timey=0;
    time3=0;
  }


  delay(20);                             //  take a break
  Serial.print("xintiao:"); 
  Serial.print(data);  
  Serial.print("        ");  
  Serial.println(fadeRate);  
  Serial.print("   timey: ");  
  Serial.print(timey);
  Serial.print("   time3: ");  
  Serial.println(time3);  
  Serial.print("   xintiao: ");  
  Serial.print(num); 
  Serial.println("num/min");  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}



void ledFadeToBeat(){
  fadeRate -= 15;                         //  set LED fade value
  fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
  //analogWrite(fadePin,fadeRate);          //  fade LED
  if(fadeRate>200)
    digitalWrite(fadePin,HIGH);
  else
    digitalWrite(fadePin,LOW);
}


























