/* Four digital tube display control
** Note that the use of 5461AS is common cathode, anode, if you use common Anode, pay attention to is the replacement of certain parts of the HIGH and LOW
*/

//Set anode Interface
//D1 port used for serial data transmission (TX)
int a = 2;//pin D0,D1 in this programm is use to data input, output
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int p = 9;//pointer

//Set cathode interface (1,2,3,4 digital control of light on and off)
int d1 = 13;//thousand digital
int d2 = 12;//hundred digital
int d3 = 11;//ten digital
int d4 = 10;//个位single digital


int del = 5000;  //This value can be used to fine-tune the clock
int changepin = A0;//Potentiometer input data from A0 mouth
int val=0;//Receiving a count value obtained from a potential opening A0
int val4=0;  //pin13 thousand
int val3=0;  //pin12 hundred
int val2=0;  //pin11 ten
int val1=0;  //pin10 single

void setup()
{
  Serial.begin(9600);
 
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  pinMode(p, OUTPUT);
}

void loop()
{
  val=analogRead(changepin);
  Serial.println(val);
  for(int i=0;i<25;i++)//To make digital not so sensitive come and go, giving him 25 cycles just to satisfy my needs
  {
    
    if(val>=1000)//Ten thousand
    {

      val4=(val/1000)%10;
      val3=( val-((val/1000)%10*1000) )/100%10;
      val2=(val-val/100%10*100)/10%10;
      val1= val-val/10%10*10-val/1000%10*1000;

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(3);
      pickNumber(val3);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(4);
      pickNumber(val4);
      delayMicroseconds(del);
    }
    else if(val>=100 && val<1000)//thousand
    {
      val3=(val/100)%10;
      val2=((val-(((val/100)%10)*100))/10)%10;
      val1=val-((val/100)%10)*100-((((val-((val/100)%10)*100)/10)%10)*10);

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(2*del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(3);
      pickNumber(val3);
      delayMicroseconds(del);  
    }
    else if(val>=10 && val<100)//hundred
    {
      val2=(val/10)%10;
      val1=val-(((val/10)%10)*10);

      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);

      clearLEDs();
      pickDigit(2);
      pickNumber(val2);
      delayMicroseconds(del);
    }
    else if(val>=0 && val<10)//ten
    {
      val1=val;
      clearLEDs();
      pickDigit(1);
      pickNumber(val1);
      delayMicroseconds(del);
    }
  }
}

void pickDigit(int x)
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  if(x==1)
  {
    digitalWrite(d4, LOW);
  }
  else if(x==2)
  {
    digitalWrite(d3, LOW);
  }
  else if(x==3)
  {
    digitalWrite(d2, LOW);
  }
  else if(x==4)
  {
    digitalWrite(d1, LOW);
  }
}

void pickNumber(int x)
{
  switch(x)
  {
  default:
    zero();
    break;
  case 1:
    one();
    break;
  case 2:
    two();
    break;
  case 3:
    three();
    break;
  case 4:
    four();
    break;
  case 5:
    five();
    break;
  case 6:
    six();
    break;
  case 7:
    seven();
    break;
  case 8:
    eight();
    break;
  case 9:
    nine();
    break;
  }
}

void clearLEDs()  //clear screen
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);

  digitalWrite(p, LOW);
}



void zero()  //0
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one()  //1
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two()  //2
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void three()  //3
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four()  //4
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five()  //5
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six()  //6
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven()  //7
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight()  //8
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine()  //9
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void dpoint() //pointer
{
  digitalWrite(p, HIGH);
}
