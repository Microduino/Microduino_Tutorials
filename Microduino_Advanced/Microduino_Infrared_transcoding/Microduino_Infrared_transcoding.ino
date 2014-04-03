#include <IRremote.h>

int RECV_PIN = A3;

int choose;

IRsend irsend;
IRrecv irrecv(RECV_PIN);
decode_results results;
/*---------------------------------------------------------------------------------*/
void setup()
{ pinMode(A3,INPUT);
  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);  
  irrecv.enableIRIn(); 
}
/*---------------------------------------------------------------------------------*/
void hongwaikong()
{
  unsigned long remote = results.value;	//set iR signal is remote

    Serial.println(remote,HEX);
  if(remote==0xFF906F||remote==0xFD00FF) 
    choose=1;
  else if(remote==0xFF38C7||remote==0xFD08F7)        
    choose=2;
  else if(remote==0xFF10EF||remote==0xFD8877)        
    choose=3;
  else if(remote==0xFF5AA5||remote==0xFD48B7)       
    choose=4;
  else if(remote==0xFF18E7||remote==0xFDC837)  
    choose=5;
  else if(remote==0xFF4AB5||remote==0xFD28D7) 
    choose=6;
  else
    choose=0;

  irrecv.resume();  
}
/*---------------------------------------------------------------------------------*/
void hongwaisend()
{
  switch(choose)
  {
  case 1:
    {
      irsend.sendNEC(0x19F658A7, 32);
      digitalWrite(11,HIGH);
    }
    break;
  case 2:
    {
      irsend.sendNEC(0x19F620DF, 32);
      digitalWrite(11,HIGH);
    }
    break;
  case 3:
    {
      irsend.sendNEC(0x19F6807F, 32);
      digitalWrite(11,HIGH);
    }
    break;
  case 4:
    {
      irsend.sendNEC(0x19F610EF, 32);
      digitalWrite(11,HIGH);
    }
    break;
  case 5:
    {
      irsend.sendNEC(0x19F6906F, 32);
      digitalWrite(11,HIGH);
    }
    break;
  case 6:
    {
      irsend.sendNEC(0x19F6C03F, 32);
      digitalWrite(11,HIGH);
    }
    break;

  }
  irrecv.enableIRIn();      //reset server

  delay(500);
  digitalWrite(11,LOW);

}
/*---------------------------------------------------------------------------------*/

void loop()
{
  if(irrecv.decode(&results))
  {
    hongwaikong();
    delay(100);
    hongwaisend();
  }

}
