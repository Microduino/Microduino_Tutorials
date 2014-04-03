#include "U8glib.h"


int potPin = A0;//Potentiometer pin definitions
int motorPin = 9;//Motor pin definitions
int potValue = 0;//The value of the potentiometer
int motorValue = 0;//Value of the motor


int NbTopsFan=0;
int Calc=0;

int hallsensor = 2;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//The definition OLED connection


void rpm ()      //This is the function that the interupt calls
{
NbTopsFan++;
}


//show speed
void draw() {

  u8g.setFont(u8g_font_courB14);//font
  u8g.drawStr( 0, 20, "Speed:");
  u8g.setPrintPos(0, 54);
  u8g.print(Calc);
  u8g.print(" (rpm)");
}  


void setup()
{
  pinMode(hallsensor, INPUT);
Serial.begin(9600);
attachInterrupt(0, rpm, RISING);
}


void loop()
{
  //Reading the value of the potentiometer
  potValue = analogRead(potPin);  
  //Mapped to the value of the motor speed
  motorValue = map(potValue, 0, 1023, 0, 200);
  analogWrite(motorPin, motorValue);



   NbTopsFan = 0; //Set NbTops to 0 ready for calculations
   sei(); //Enables interrupts
   delay (1000); //Wait 1 second
   cli(); //Disable interrupts
   //Calc = ((NbTopsFan * 60)/fanspace[fan].fandiv); //Times NbTopsFan (which is apprioxiamately the fequency the fan is spinning at) by 60 seconds before dividing by the fan's divider
   Calc = (NbTopsFan * 60); 
   Serial.print (Calc, DEC); //Prints the number calculated above
   Serial.print (" rpm\r\n"); //Prints " rpm" and a new line



  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
}

