#include <Wire.h>
#include <AM2321.h>
AM2321 am2321;


//latchpin connect to pin10
int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 11;

long  duan[]={
  0x07,0x0b,0x0d,0x0e,0x07,0x0b,0x0d,0x0e
};

long  data[10]=
{
  //0,1,2,3,4,5,6,7,8,9,`,c
  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};

long signal[3]={
  //`,c,.
  0x63,0x39,0x5C
};

int showInfo[4]={
//inital 0000
  0x3F,0x3F,0x3F,0x3F};


int count=0;


void setup(void)
{
  pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(latchPin, OUTPUT);
}



void loop(void)
{

  if(count==1000) count=0;
  
  //read sensor data
  am2321.read();
  int num=0;

  if(count<500) {
  //read tempture
    num=am2321.temperature/10.0;
	//set symbol ·c
    showInfo[1]=signal[0];
    showInfo[0]=signal[1];
  } 
  else {
  //read humidity
    num=am2321.humidity/10.0;
	//set sybmol %
    showInfo[1]=signal[0];
    showInfo[0]=signal[2];
  }
  //get ten ditital
  int tenDigit=num/10;
  //get singal digital
  int singleDigit=num%10;

  //set ten digital
  showInfo[3]=data[tenDigit];
  //set single digital
  showInfo[2]=data[singleDigit];

  //four digital
  for(int i=3;i>=0;i--)
  {
    digitalWrite(latchPin, 0);
    ledduan(duan[i]);
    shiftOut(dataPin, clockPin,showInfo[i]);
    digitalWrite(latchPin, 1);
    delay(2); 
  }  

  count++;
      
}


void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {

  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {        
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  digitalWrite(myClockPin, 0);
}

void ledduan(int num)   
{
  digitalWrite(8, ((num>>0)&0x01));
  digitalWrite(4, ((num>>1)&0x01));
  digitalWrite(3, ((num>>2)&0x01));
  digitalWrite(1, ((num>>3)&0x01));
}

