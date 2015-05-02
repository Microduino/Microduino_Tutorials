/*

Key Hexadecimal value Decimal value
KEY_LEFT_CTRL 0x80  128
KEY_LEFT_SHIFT  0x81  129
KEY_LEFT_ALT  0x82  130
KEY_LEFT_GUI  0x83  131
KEY_RIGHT_CTRL  0x84  132
KEY_RIGHT_SHIFT 0x85  133
KEY_RIGHT_ALT 0x86  134
KEY_RIGHT_GUI 0x87  135
KEY_UP_ARROW  0xDA  218
KEY_DOWN_ARROW  0xD9  217
KEY_LEFT_ARROW  0xD8  216
KEY_RIGHT_ARROW 0xD7  215
KEY_BACKSPACE 0xB2  178
KEY_TAB 0xB3  179
KEY_RETURN  0xB0  176
KEY_ESC 0xB1  177
KEY_INSERT  0xD1  209
KEY_DELETE  0xD4  212
KEY_PAGE_UP 0xD3  211
KEY_PAGE_DOWN 0xD6  214
KEY_HOME  0xD2  210
KEY_END 0xD5  213
KEY_CAPS_LOCK 0xC1  193
KEY_F1  0xC2  194
KEY_F2  0xC3  195
KEY_F3  0xC4  196
KEY_F4  0xC5  197
KEY_F5  0xC6  198
KEY_F6  0xC7  199
KEY_F7  0xC8  200
KEY_F8  0xC9  201
KEY_F9  0xCA  202
KEY_F10 0xCB  203
KEY_F11 0xCC  204
KEY_F12 0xCD  205
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5); //RX,TX

//#define my_Serial mySerial  //core
#define my_Serial Serial1  //Core+


String currentInfo;

void setup(void) {
  Serial.begin(9600);

  my_Serial.begin(9600); 

  Keyboard.begin();

}

void loop() {

  if (my_Serial.available() > 0) {
    currentInfo = my_Serial.readStringUntil('\n');
    Serial.println(currentInfo);
    if(currentInfo=="ERROR"||currentInfo=="Connected") {
      return;
    } else if (currentInfo=="off") {
      //shoutDown();
    }
  }


}

void shoutDown()
{

//windows XP的关机快捷键为win+u+u(输入法要在英文状态下)

//windows 7的关机快捷键为win+->(右键头)+Enter

//windows 8快捷键没有,不过可以在桌面状态下按下Alt+F4就可关机了


/*
//windows XP
  Keyboard.press(KEY_LEFT_GUI);
  delay(100);
  Keyboard.write('u');
  delay(100);
  Keyboard.write('u');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);
*/

/*
//windows 7
  Keyboard.press(KEY_LEFT_GUI);
  delay(100);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(100);
  Keyboard.write(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  delay(1000);
*/


//windows 8

  Keyboard.press(KEY_LEFT_GUI);
  delay(100);
  Keyboard.write('x');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);


  Keyboard.write(KEY_UP_ARROW);
  delay(100);
  Keyboard.write(KEY_UP_ARROW);
  delay(100);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(100);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(100);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(100);
  Keyboard.write(KEY_RETURN);

  delay(1000);

}
