#include "U8glib.h"
#include <Keypad.h>
#include <avr/eeprom.h>

#include <Servo.h>
Servo myservo; // create servo object to control a servo
// a maximum of eight servo objects can be created
int pos = 0; // variable to store the servo position 

#define EEPROM_write(address, var) eeprom_write_block((const void *)&(var), (void *)(address), sizeof(var))
#define EEPROM_read(address, var) eeprom_read_block((void *)&(var), (const void *)(address), sizeof(var))

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

unsigned long currentPass;

char customKey;
char charBuf[10];
// start reading from the first byte (address 0) of the EEPROM
int address = 0;

unsigned long clearPass=NULL;
unsigned long initPass=123456;

unsigned long readPass;
unsigned long writePass;

unsigned long convertNumber;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  { 
    '1','2','3','A'    }
    ,
    { 
      '4','5','6','B'    }
      ,
      { 
        '7','8','9','C'    }
        ,
        { 
          '*','0','#','D'    }
        };
        byte rowPins[ROWS] = {
9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
5,4,3,2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	

int servoPin = 10,Alarm = 11;

String txtMsg = "";   

String passwordNumber;

int ok;
#define TIME 500 
long time1=0,time2=0;

void setup() {
  Serial.begin(9600);
pinMode(servoPin, OUTPUT);      // sets the digital pin as output
customKeypad.addEventListener(keypadEvent); //add an event listener for this keypad

myservo.attach(10); // attaches the servo on pin 10 to the servo object 

EEPROM_read(address,readPass);

if(readPass==NULL||readPass==0) {
  EEPROM_write(address, initPass);
  Serial.println("init:");
  currentPass=initPass;
  Serial.println(currentPass);
}else {
  EEPROM_read(address,readPass);
  currentPass=readPass;
  Serial.println("read:");
  Serial.println(currentPass);
}


}

void draw(void) {
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(0, 16); 
  u8g.print("Password:");
  //u8g.print(txtMsg);
  u8g.setPrintPos(62, 16); 
  //switch (String(txtMsg).length()) {
  switch (txtMsg.length()) {
    case 1:
    u8g.print("*");
    break;
    case 2:
    u8g.print("**");
    break;
    case 3:
    u8g.print("***");
    break;
    case 4:
    u8g.print("****");
    break;
    case 5:
    u8g.print("*****");
    break;
  }
  if(millis()<time2+TIME) {
    u8g.print("_");
    time1=millis();
  } else {
    u8g.print(" ");
    if(millis()>time1+TIME)
      time2=millis();
  }

  if(ok==1) {//entry pass
    u8g.setFont(u8g_font_9x18B);
    u8g.setPrintPos(30, 40); 
    u8g.print("Welcome!");
  } else if(ok==0) {//type password
    u8g.setFont(u8g_font_9x18B);
    u8g.setPrintPos(30, 40); 
    u8g.print("       ");
  } else if (ok==2) {//change password, type old password first
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(0, 30); 
    u8g.print("change your password?");
    u8g.setPrintPos(0, 46); 
    u8g.print("Type your old password:");
  } else if (ok==3) {//entry new password
    u8g.setFont(u8g_font_7x14);
    u8g.setPrintPos(0, 32); 
    u8g.print("change new password?");
  } else if (ok==4) {//type wrong password
    u8g.setFont(u8g_font_7x14);
    u8g.setPrintPos(0, 32); 
    u8g.print("not right");
    delay(3000);
    ok=2;
  }

}
void loop(){
  customKey = customKeypad.getKey();
  //customKeypad.getKey();

  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}

void keypadEvent(KeypadEvent eKey) {
  if(customKeypad.getState()==PRESSED) {
    delay(10);

    if(eKey=='C') {
      txtMsg="";
    } else if(eKey=='D') {
      ok=0;
      txtMsg="";
    } else if(eKey=='*') {
      ok=2;
      txtMsg="";
    } else {
      txtMsg += eKey;
      if(txtMsg.length()>6) {
        txtMsg="";
      }
      if(ok==0||ok==1) {
        txtMsg.toCharArray(charBuf,10);
        convertNumber=atol(charBuf);
        if(convertNumber==currentPass) {
          ok=1;
          txtMsg="";
          //control Servo
          myservo.write(90); // tell servo to go to position in variable 'pos'
          delay(5000); // waits 5s for the servo to reach the position
          myservo.write(0); // tell servo to go back 0 pos

        } 

} else if(ok==2){//type old password

  Serial.println(txtMsg);
  if(String(txtMsg).length()==6) {
    //Serial.println("length==6");
    txtMsg.toCharArray(charBuf,10);
    Serial.println(charBuf);
    convertNumber=atol(charBuf);
    Serial.println(convertNumber);
    if(currentPass==convertNumber) {
      //Serial.print("old pass:");
      //Serial.println(txtMsg);
      ok=3;
      txtMsg="";
    } else {
      ok=4;
      //Serial.println("entry 4");
    }
  }
} else {//change password
  Serial.println(txtMsg);
  if(String(txtMsg).length()==6) {
    txtMsg.toCharArray(charBuf,10);
    convertNumber=atol(charBuf);
    writePass=convertNumber;
    EEPROM_write(0, writePass);
    currentPass=convertNumber;
    txtMsg="";
    ok=1;
  }
}
}
}
}

