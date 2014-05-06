//Written by Dean Reading, 2012.  deanreading@hotmail.com

/*
This example is a centi-second counter to demonstrate the
 use of my SevSeg library.
 */

#include "SevSeg.h"

//Create an instance of the object.
SevSeg sevseg;

//Create global variables
unsigned long timer;
int CentSec=0;

void setup() {
  //I am using a common anode display, with the digit pins connected
  //from 2-5 and the segment pins connected from 6-13
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
  timer=millis();
}

void loop() {
  //Produce an output on the display
  sevseg.PrintOutput();

  //Check if 10ms has elapsed
  unsigned long mils=millis();
  if (mils-timer>=10) {
    timer=mils;
    CentSec++;
    if (CentSec==10000) { // Reset to 0 after counting for 100 seconds.
      CentSec=0;
    }
    //Update the number to be displayed, with a decimal
    //place in the correct position.
    sevseg.NewNum(CentSec,(byte) 2);
  }
}






