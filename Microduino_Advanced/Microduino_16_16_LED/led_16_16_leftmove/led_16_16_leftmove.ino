int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 11;

char temp,temp_1,temp1,temp2,temp3,temp4,temp_c;

long col[]={
  0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
long cle[]={
  0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xff};

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
  for(int k=0;k<16;k++)
  {
    if(k<8)
    {
      for(int j=0;j<50;j++)
      {
        //上部分
        for(int i=0;i<8;i++)
        {
          temp1=row[i+16];
          temp2=row[i];
          switch(k)
          {
          case 0:	 
            temp=(temp2>>1&0x7f)|(temp1<<7);
            break;
          case 1:	 
            temp=(temp2>>2&0x3f)|(temp1<<6);	
            break;
          case 2:	 
            temp=(temp2>>3&0x1f)|(temp1<<5);
            break;
          case 3:	 
            temp=(temp2>>4&0x0f)|(temp1<<4);
            break;
          case 4:	 
            temp=(temp2>>5&0x07)|(temp1<<3);
            break;
          case 5:	 
            temp=(temp2>>6&0x03)|(temp1<<2);
            break;
          case 6:	 
            temp=(temp2>>7&0x01)|(temp1<<1);
            break;
          case 7:	 
            temp=temp1;   
            break;
          }
          digitalWrite(latchPin, 0);
          shiftOut(dataPin, clockPin, col[i]);  //mask(col)   
          shiftOut(dataPin, clockPin, col[i]&0x00);
          shiftOut(dataPin, clockPin, row[i+16]>>k+1|cle[k]);  //row
          shiftOut(dataPin, clockPin, temp);  //row
          digitalWrite(latchPin, 1);
        }
        //下部分
        for(int i=0;i<8;i++)
        {
          temp1=row[i+24];
          temp2=row[i+8];
          switch(k)
          {
          case 0:	 
            temp_1=(temp2>>1&0x7f)|(temp1<<7);
            break;
          case 1:	 
            temp_1=(temp2>>2&0x3f)|(temp1<<6);	
            break;
          case 2:	 
            temp_1=(temp2>>3&0x1f)|(temp1<<5);
            break;
          case 3:	 
            temp_1=(temp2>>4&0x0f)|(temp1<<4);
            break;
          case 4:	 
            temp_1=(temp2>>5&0x07)|(temp1<<3);
            break;
          case 5:	 
            temp_1=(temp2>>6&0x03)|(temp1<<2);
            break;
          case 6:	 
            temp_1=(temp2>>7&0x01)|(temp1<<1);
            break;
          case 7:	 
            temp_1=temp1;   
            break;
          }
          digitalWrite(latchPin, 0);
          shiftOut(dataPin, clockPin, col[i]&0x00);
          shiftOut(dataPin, clockPin, col[i]);  //mask(col)   
          shiftOut(dataPin, clockPin, row[i+24]>>k+1|cle[k]);  //row
          shiftOut(dataPin, clockPin, temp_1);  //row
          digitalWrite(latchPin, 1);
        }
      }
    }
    else
    {
      for(int j=0;j<50;j++)
      {
        for(int i=0;i<8;i++)
        {
          digitalWrite(latchPin, 0);
          shiftOut(dataPin, clockPin, col[i]);  //mask(col)   
          shiftOut(dataPin, clockPin, col[i]&0x00);
          shiftOut(dataPin, clockPin, 0xff);  //row
          shiftOut(dataPin, clockPin, row[i+16]>>k-7|cle[k-8]);  //row
          digitalWrite(latchPin, 1);
        }
        //下部分
        for(int i=0;i<8;i++)
        {
          digitalWrite(latchPin, 0);
          shiftOut(dataPin, clockPin, col[i]&0x00);  //mask(col)   
          shiftOut(dataPin, clockPin, col[i]);
          shiftOut(dataPin, clockPin,0xff);  //row  
          shiftOut(dataPin, clockPin, row[i+24]>>k-7|cle[k-8]);  //row
          digitalWrite(latchPin, 1);
        }
      }
    }

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






