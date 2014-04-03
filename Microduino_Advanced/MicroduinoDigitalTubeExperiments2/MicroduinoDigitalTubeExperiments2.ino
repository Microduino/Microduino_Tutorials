    /*

Specific performance 7 digital control method is as follows:
    A
   ____
  |    |
F |____| B
  | G  |
E |____| C
        o DP
    D
    
pin num layout:
10 9 8 7 6
________
|  __   |
| |__|  |
| |__|o |
|_______|
1 2 3 4 5

* How connect Microduino on it?
* First look at these two pins 3 and 8, generally is + or -.
* 1 (E) connect to Microduino's pin10
* 2 (D) connect to Microduino's pin9
* 3 Because we only use a digital control, so set the pin or 8-pin connect to Microduino digital pin5
* 4 (C) connect to Microduino pin8
* 5 (DP) connect to Microduino pin13
* 6 (B) connect to Microduino pin7
* 7 (A) connect to Microduino pin6
* 8 Because we only use a digital control, so set the pin or 8-pin connect to Microduino digital pin5
* 9 (F) connect to Microduino pin11
* 10 (G) connect to Microduino pin12
*/
     
    #include "SevSeg.h"
    SevSeg sevseg;
    void setup() {
    //define use pin
      sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
     
    }
     
    void loop() {
      //enable pritoutput
		sevseg.PrintOutput();
		int val = analogRead(A0);
		val = map(val,0,1023,0,9);    //use map function to do maping
        sevseg.NewNum(val, 4);     //Function digital output figures, the value of which is the location of the decimal point, I placed fourth grid so the effect is not displayed decimal
      }
