/*                               
使用一个74HC595移位寄存器代码
    计数从0到255的
*/

//Pin 8 连接到 74HC595的ST_CP
int latchPin = 8;
//Pin 12 连接到  74HC595的SH_CP
int clockPin = 12;
////Pin 11 连接到  74HC595的DS
int dataPin = 11;



void setup() {
  //设置 pins 为 output 以便于你能控制移位寄存器
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // 在LED上显示2进制数字，从0到255
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // 设置 latchPin 为低电平 
    digitalWrite(latchPin, LOW);
    // 输入十进制数字
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //设置 latch pin 为高电平，LED将显示对应的二进制数字
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}
