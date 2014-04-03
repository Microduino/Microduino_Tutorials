    /*
	Microduino Analog Keyboard
   
    Description: Plug this analog keyboard, open Notepad, and then press the button to print out the words "HELLO WORLD" in Notepad 
     
    Wiring：
    Microduino D2 connect to 68Ω，and connect to D+ of USB
    Microduino D4 connect to  68Ω and connect to D- of USB
    Microduino D5 connect to  2.2kΩ and connect to D- of USB
    Microduino D2 connect to 3.6v Diode， end to GND
    Microduino D4 connect to 3.6v Diode， end to GND
    +5v connect to VCC of USB
    GND connect to GND of USB
    Microduino D1 connect a switch to GND
     
    
    Attached: USB cable sequence colors (because various production plants, not necessarily accurate, for reference only)
    *USB mouse：      |        *USB interface
    white<->VCC        |        red<->VCC
    orange<->D-         |        white<->D-
    green<->D+         |        green<->D+
    blue<->GND        |        black<->GND
     
    */
     
    #include "UsbKeyboard.h"
    int KEYPIN = 1;                //button can connect to pin1, or any pin which you wnat
    void setup()
    {
      TIMSK0 &= !(1 << TOIE0);        
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
