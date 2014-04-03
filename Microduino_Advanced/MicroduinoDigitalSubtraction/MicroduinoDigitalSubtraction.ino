    #include "SevSeg.h"
    SevSeg sevseg;
    int i=5;
    
    void setup() {
    //define pin
      sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
      pinMode(2, INPUT);    //D2 use to add
      pinMode(3, INPUT);    //D3 use to sub
      
      sevseg.PrintOutput();
      sevseg.NewNum(i, 4); 
      
    }
     
    void loop() {
      //enable printout
      sevseg.PrintOutput();

      if(digitalRead(2)==HIGH) {//if button 2 push
        i++;
        sevseg.NewNum(i, 4);
        delay(200);        //delay 50ms
      }
      else if(digitalRead(3)==HIGH) {//if button 3 push
        i--;
        sevseg.NewNum(i, 4);
        delay(200);        //delay 50ms
      }

  }
