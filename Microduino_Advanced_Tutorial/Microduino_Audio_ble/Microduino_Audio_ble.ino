byte key0[4] ={
  0x7E,0x02,0x0D,0xEF};//play
byte key1[4] ={
  0x7E,0x02,0x0E,0xEF};//stop
byte key2[4] ={
  0x7E,0x02,0x02,0xEF};//up
byte key3[4] ={
  0x7E,0x02,0x01,0xEF};//down
byte key4[4] ={
  0x7E,0x02,0x04,0xEF};//vol+
byte key5[4] ={
  0x7E,0x02,0x05,0xEF};//vol-

byte key6[5] ={
  0x7E,0x03,0x09,0x01,0xEF};//device select "tf"   U/TF/AUX/SLEEP/FLASH      00-05
byte key7[5] ={
  0x7E,0x03,0x11,0x00,0xEF};//play mode     "all"  ALL FOL ONE RAM ONE_STOP  00-05
byte key8[5]={
  0x7E,0x03,0x06,0x18,0xEF};//vol 24 0-1E(0-30)-16,8,4,2,1

#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //RX,TX

String comdata = "";  //显示的字符串

int pinRx = 4;
int pinTx = 5;

void setup()
{  
  Serial.begin(9600);
  mySerial.begin(9600); 
  pinMode(pinTx,OUTPUT);
  pinMode(pinRx,INPUT_PULLUP);
  for(int a=0;a<5;a++)Serial.write(key8[a]);  //vol 18
  delay(300);
  for(int a=0;a<5;a++)Serial.write(key6[a]);  //tf card
  delay(300);
  for(int a=0;a<5;a++)Serial.write(key7[a]);  //play all
  delay(600);
  while(!Serial)
  {
  }
}

void loop()
{
  while (mySerial.available() > 0)  //判断串口是否有输入
  {
    comdata += char(mySerial.read()); //读取字符
    delay(2);                         //等待串口缓存
  }
  if(comdata=="play")
    for(int a=0;a<4;a++)Serial.write(key0[a]);
  else if(comdata=="stop")
    for(int a=0;a<4;a++)Serial.write(key1[a]);
  else if(comdata=="up")
    for(int a=0;a<4;a++)Serial.write(key2[a]);
  else if(comdata=="down")
    for(int a=0;a<4;a++)Serial.write(key3[a]);
  else if(comdata=="vol+")
    for(int a=0;a<4;a++)Serial.write(key4[a]);
  else if(comdata=="vol-")
    for(int a=0;a<4;a++)Serial.write(key5[a]);

  comdata = "";
}






