#include "SevSeg.h"
SevSeg sevseg;

void setup() {
  //define pin
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
  pinMode(1, INPUT);    //button1---pin1
  pinMode(2, INPUT);    //button2---pin2
  pinMode(3, INPUT);    //button2---pin2
  pinMode(4, INPUT);    //button2---pin2

  sevseg.PrintOutput();
  sevseg.NewNum(0, 4); //inital 0

}

void loop() {
  //enable printout function
  sevseg.PrintOutput();

  //Loop reads these four buttons pin
  for(int i=1; i<=4; i++) {
    if(digitalRead(i)==HIGH) {//if push button
      while(1) {//show button number
        sevseg.PrintOutput();
        sevseg.NewNum(i, 4);
      }
    }

  }




}


