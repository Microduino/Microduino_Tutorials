#include <IRremote.h>                  // 引用 IRRemote 函式库

IRsend irsend;                          // 定义 IRsend 物件来发射红外线讯号
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  

  digitalWrite(ledPin, HIGH);   // 点亮LED测试（需通过手机摄像模式观察）
  delay(3000);             // 等待3秒
  digitalWrite(ledPin, LOW);    // 结束测试  
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {    
    Serial.print("SendIR: ");
    irsend.sendNEC(0x234E817, 32); //9
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
