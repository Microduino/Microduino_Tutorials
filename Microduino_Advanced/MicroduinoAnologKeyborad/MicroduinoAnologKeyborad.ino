    /*
	Microduino模拟键盘
   
    功能描述：插上此模拟键盘，打开记事本，然后按下按钮，即可在记事本打印出HELLO WORLD字样
     
    接线方法：
    Microduino D2接68Ω电阻后，接USB线D+
    Microduino D4接68Ω电阻后，接USB线D-
    Microduino D5接2.2kΩ电阻后，接USB线D-
    Microduino D2接3.6v稳压管到GND
    Microduino D4接3.6v稳压管到GND
    +5v接USB线VCC
    GND接USB线GND
    Microduino D1接一个开关到GND
     
    附：USB线序颜色（由于各生产厂不同，不一定准确，仅供参考）
    *USB键鼠：      |        *USB接口
    白<->VCC        |        红<->VCC
    橙<->D-         |        白<->D-
    绿<->D+         |        绿<->D+
    蓝<->GND        |        黑<->GND
     
    */
     
    #include "UsbKeyboard.h"
    int KEYPIN = 1;                //按键接在D1引脚，也可以改成任何引脚
    void setup()
    {
      TIMSK0 &= !(1 << TOIE0);        //
      pinMode(KEYPIN, INPUT);
      digitalWrite(KEYPIN, HIGH);
    }
    void loop()
    {
      UsbKeyboard.update();
      if(digitalRead(KEYPIN) == HIGH)
      {
        delay(100);
        if(digitalRead(KEYPIN) == LOW)
        {
          UsbKeyboard.sendKeyStroke(KEY_H);
          UsbKeyboard.sendKeyStroke(KEY_E);
          UsbKeyboard.sendKeyStroke(KEY_L);
          UsbKeyboard.sendKeyStroke(KEY_L);
          UsbKeyboard.sendKeyStroke(KEY_O);
          UsbKeyboard.sendKeyStroke(KEY_SPACE);
          UsbKeyboard.sendKeyStroke(KEY_W);
          UsbKeyboard.sendKeyStroke(KEY_O);
          UsbKeyboard.sendKeyStroke(KEY_R);
          UsbKeyboard.sendKeyStroke(KEY_L);
          UsbKeyboard.sendKeyStroke(KEY_D);
          UsbKeyboard.sendKeyStroke(KEY_ENTER);
        }
      }
    }
