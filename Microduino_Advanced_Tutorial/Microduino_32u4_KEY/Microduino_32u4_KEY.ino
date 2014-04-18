/*
#define KEY_LEFT_CTRL		0x80
 #define KEY_LEFT_SHIFT		0x81
 #define KEY_LEFT_ALT		0x82
 #define KEY_LEFT_GUI		0x83
 #define KEY_RIGHT_CTRL		0x84
 #define KEY_RIGHT_SHIFT		0x85
 #define KEY_RIGHT_ALT		0x86
 #define KEY_RIGHT_GUI		0x87
 
 #define KEY_UP_ARROW		0xDA
 #define KEY_DOWN_ARROW		0xD9
 #define KEY_LEFT_ARROW		0xD8
 #define KEY_RIGHT_ARROW		0xD7
 #define KEY_BACKSPACE		0xB2
 #define KEY_TAB				0xB3
 #define KEY_RETURN			0xB0
 #define KEY_ESC				0xB1
 #define KEY_INSERT			0xD1
 #define KEY_DELETE			0xD4
 #define KEY_PAGE_UP			0xD3
 #define KEY_PAGE_DOWN		        0xD6
 #define KEY_HOME			0xD2
 #define KEY_END				0xD5
 #define KEY_CAPS_LOCK		        0xC1
 #define KEY_F1				0xC2
 #define KEY_F2				0xC3
 #define KEY_F3				0xC4
 #define KEY_F4				0xC5
 #define KEY_F5				0xC6
 #define KEY_F6				0xC7
 #define KEY_F7				0xC8
 #define KEY_F8				0xC9
 #define KEY_F9				0xCA
 #define KEY_F10				0xCB
 #define KEY_F11				0xCC
 #define KEY_F12				0xCD
 */

const int encoderPinA = 2;
const int encoderPinB = 3;

boolean encoderALast = LOW;  // remembers the previous pin state

#define E_enter 6


boolean key1;			//按键
boolean key1_cache;		//检测按键松开缓存


void setup() {
  // make pin 2 an input and turn on the 
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(encoderPinA,INPUT_PULLUP); //D2脚为输入
  pinMode(encoderPinB,INPUT_PULLUP); //D3脚为输入
  pinMode(E_enter,INPUT_PULLUP); //D3脚为输入

  attachInterrupt(4, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2


  delay(2000);
  Keyboard.begin();
}

void loop() {

  //KEY============================
  key1=digitalRead(E_enter);

  if(key1 && key1_cache)		//按下松开后
  {

    Keyboard.press(KEY_LEFT_CTRL);
    delay(100);
    Keyboard.press(KEY_TAB);
    delay(100);
    Keyboard.releaseAll();
    delay(100);

  }

  key1_cache=!key1;		//缓存作判断用


}



void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) 
  {
        Keyboard.press(KEY_UP_ARROW);
        Keyboard.releaseAll();
        delay(10);
  }
  else 
  {
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.releaseAll();
      delay(10);
  }
}

