    /*  

    Specific performance 7 digital control method is as follows:
          
               A
              ____
             |    |
          F  |____|  B
             | G  |
          E  |____|  C
                    o DP
               D
              
          pin num layout:
          
		   g f   a b
          10 9 8 7 6
           ________
          |   __   |
          |  |__|  |
          |  |__|o |
          |________|
          
          1 2 3 4 5  
          e d   c dp
          * How connect Microduino CoreSTM32 on it?
          * First look at these two pins 3 and 8, generally is + or -.
          * a   connect to Microduino's pin2
          * b   connect to Microduino's  pin3       
          * c   connect to Microduino's pin4
          * d   connect to Microduino's pin5
          * e   connect to Microduino's pin6
          * f   connect to Microduino's pin7
          * g   connect to Microduino's pin8
          * dp  connect to Microduino's pin9
          * 3/8 Because we only use a digital control, so set the pin or 8-pin connect to Microduino's digital pin10  
          
    */

int a=2;   int b=3;   int c=4;   int d=5;    //define needed pin 
int e=6;   int f=7;   int g=8;   int dp=9;   //a,b,c,d,e,f,g,dp
int d1=10; int d2=11; int d3=12; int d4=13;  //digital control
void initSegDisplay()
{
  // set the digital pin as output
  pinMode(a,OUTPUT);  pinMode(b,OUTPUT); 
  pinMode(c,OUTPUT);  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);  pinMode(dp,OUTPUT);
  pinMode(d1,OUTPUT); pinMode(d2,OUTPUT); 
  pinMode(d3,OUTPUT); pinMode(d4,OUTPUT); 
}
// num：the value to display 
// x:the segment number 
void DisplayS(int num,int x) 
{
  digitalWrite(d1,LOW); digitalWrite(d2,LOW);//清除每位
  digitalWrite(d3,LOW); digitalWrite(d4,LOW);
  if(x==1)
  {digitalWrite(d1,HIGH);} //The frist segment
  else if(x==2)
  {digitalWrite(d2,HIGH);} //The second segment
  if(x==3)
  {digitalWrite(d3,HIGH);} //The third segment
  else if(x==4)
  {digitalWrite(d4,HIGH);} //The four segment
  if(num==0)  //display number 0
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,HIGH);digitalWrite(dp,HIGH);
  }
  else if(num==1) //display number 1
  {
    digitalWrite(a,HIGH); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,HIGH);
    digitalWrite(e,HIGH); digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);digitalWrite(dp,HIGH);
  }
  else if(num==2) //display number 2
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,HIGH); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,HIGH);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==3) //display number 3
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,HIGH); digitalWrite(f,HIGH);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==4) //display number 4
  {
    digitalWrite(a,HIGH); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,HIGH);
    digitalWrite(e,HIGH); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==5) //display number 5
  {
    digitalWrite(a,LOW); digitalWrite(b,HIGH);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,HIGH); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==6) //display number 6
  {
    digitalWrite(a,LOW); digitalWrite(b,HIGH);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==7) //display number 7
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,HIGH);
    digitalWrite(e,HIGH); digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);digitalWrite(dp,HIGH);
  }
  else if(num==8) //display number 8
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==9) //display number 9
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,HIGH); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }  
   else if(num==10)//display character A
  {
    digitalWrite(a,LOW); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,HIGH);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
   else if(num==11)//display character b
  {
    digitalWrite(a,HIGH); digitalWrite(b,HIGH);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
   else if(num==12)//display character c
  {
    digitalWrite(a,HIGH); digitalWrite(b,HIGH);
    digitalWrite(c,HIGH); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,HIGH);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==13)//display character d
  {
    digitalWrite(a,HIGH); digitalWrite(b,LOW);
    digitalWrite(c,LOW); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,HIGH);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==14)//display character E
  {
    digitalWrite(a,LOW); digitalWrite(b,HIGH);
    digitalWrite(c,HIGH); digitalWrite(d,LOW);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else if(num==15)//display character F
  {
    digitalWrite(a,LOW); digitalWrite(b,HIGH);
    digitalWrite(c,HIGH); digitalWrite(d,HIGH);
    digitalWrite(e,LOW); digitalWrite(f,LOW);
    digitalWrite(g,LOW);digitalWrite(dp,HIGH);
  }
  else //display decimal point 
  {
    digitalWrite(a,HIGH); digitalWrite(b,HIGH);
    digitalWrite(c,HIGH); digitalWrite(d,HIGH);
    digitalWrite(e,HIGH); digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);digitalWrite(dp,LOW);
  }
}
void setup()  
{
    initSegDisplay(); // set the digital pin mode：OUTPUT
}
void loop() 
{
 //display 0-9,A,b,c,d,E,F and decimal point
 for(int i=0;i<17;i++) 
  {
    DisplayS(i,1);
    delay(1000);  
  }    
}

