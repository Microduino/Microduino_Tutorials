int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 11;

long col[]={
  0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

long row[]={

0xFF,0x81,0xDF,0xDB,0x5B,0x9B,0xDD,0x01,
0xCF,0xD7,0xD7,0xDB,0xDD,0xDE,0xD7,0xEF,
0xF5,0xED,0xFE,0x80,0xF6,0xF6,0xC0,0xF6,
0xF6,0xC0,0xF6,0xF6,0xF6,0x80,0xFE,0xFE,//雅0
/*
0xF7,0x77,0xF7,0xF7,0xC0,0xF7,0x73,0xE3,
0xD5,0xD5,0x76,0x77,0xB7,0xD7,0xF7,0x77,
0xFF,0xE0,0xF7,0xFB,0xFD,0xFE,0x80,0xB6,
0xB6,0xB6,0xB7,0xBB,0xBB,0xBD,0xD6,0xEF,//杨",0
*/
};//行列式、阳码、逆向

void setup() {

  pinMode(latchPin, OUTPUT);

}

void loop() {
  //上部分
  for(int i=0;i<8;i++)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, col[i]);  //mask(col)   
    shiftOut(dataPin, clockPin, col[i]&0x00);
    shiftOut(dataPin, clockPin, row[i+16]);  //row
    shiftOut(dataPin, clockPin, row[i]);  //row
    digitalWrite(latchPin, 1);
  }
  //下部分
    for(int i=0;i<8;i++)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, col[i]&0x00);  //mask(col)   
    shiftOut(dataPin, clockPin, col[i]);
    shiftOut(dataPin, clockPin, row[i+24]);  //row
    shiftOut(dataPin, clockPin, row[i+8]);  //row
    digitalWrite(latchPin, 1);
  }
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




