//本例来自于红外接收模块的IRremote自带范例
/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * [url]http://arcfn.com[/url]
 */
 
#include <IRremote.h>
 
int RECV_PIN = 11;//定义红外接收器的引脚为11
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外接收器
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);//以16进制换行输出接收代码
    Serial.println();//为了便于观看输出结果增加一个空行
    irrecv.resume(); // 接收下一个值
  }
}
