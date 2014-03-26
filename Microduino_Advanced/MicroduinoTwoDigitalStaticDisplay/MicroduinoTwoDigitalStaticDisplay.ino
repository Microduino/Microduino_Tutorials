
byte digit0 = 10;	//十位数Microduino pin
byte digit1 = 11;	//个位数Microduino pin
byte dotPin = 2;	//数码管小数点对应的pin

byte sevenSegmentPins[] = {2,3,4,5,6,7,8};	//A,B,C,D,E,F,G  对应的Microduino pin
byte sevenSegment[10][7] =
{
  //a b c d e f g
  { 0,0,0,0,0,0,1 },  // = 0
  { 1,0,0,1,1,1,1 },  // = 1
  { 0,0,1,0,0,1,0 },  // = 2
  { 0,0,0,0,1,1,0 },  // = 3
  { 1,0,0,1,1,0,0 },  // = 4
  { 0,1,0,0,1,0,0 },  // = 5
  { 0,1,0,0,0,0,0 },  // = 6
  { 0,0,0,1,1,1,1 },  // = 7
  { 0,0,0,0,0,0,0 },  // = 8
  { 0,0,0,1,1,0,0 }   // = 9
};

void setup()
{

	//初始化全部亮起
   pinMode(dotPin, OUTPUT); //pin 2
   pinMode(digit0, OUTPUT); //pin 10
   pinMode(digit1, OUTPUT); //pin 11

  for(int i=0; i<7; i++)
  {
    pinMode(sevenSegmentPins[i], OUTPUT);
  }
  digitalWrite(dotPin, HIGH);
  digitalWrite(digit0, HIGH);
  digitalWrite(digit1, HIGH);
}

//显示数字
void segmentWrite(byte digit)
{
  byte pin = 2;
  for (byte i=0; i<7; ++i)
  {
    digitalWrite(pin, sevenSegment[digit][i]);
      ++pin;
  }
}

void loop()
{
  //显示 68
  digitalWrite(digit0, LOW);   //熄灭十位数码管
  segmentWrite(8);             //显示个位8
  delay(10);                  //10ms 延迟
  digitalWrite(digit0, HIGH);  //点亮十位数码管
  digitalWrite(digit1, LOW);   //熄灭个位数码管
  segmentWrite(6);             //显示十位6
  delay(10);                  //10ms 延迟
  digitalWrite(digit1, HIGH);  //点亮个位数码管

}
