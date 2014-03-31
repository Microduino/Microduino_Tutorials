#include <SimpleSDAudio.h>

int data_input=0,input_ok=0,data_output=0,output_ok=0,ok;
int data;

int push_int = A5;
int push_out = A1;

long timer;

void setup()
{ 
  Serial.begin(9600);
  pinMode(push_int, INPUT);
  pinMode(push_out, INPUT);
}
void music(int name)
{
  // If your SD card CS-Pin is not at Pin 7, enable and adapt the following line:
  // SdPlay.setSDCSPin(9);
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    while(1); // Error while initialization of SD card -> stop.
  } 
  switch(name)
  {
  case 1:
    {
      if(!SdPlay.setFile("goout.afm")) 
      {
        while(1); 
      }  
    }
    ;
    break;
  case 2:
    {
      if(!SdPlay.setFile("welcome.afm")) 
      {
        while(1); 
      }  
    }
    ;
    break;
  }

  SdPlay.play();

  while(!SdPlay.isStopped()) {
    SdPlay.worker();
  }
}

void loop(void) 
{
  int button_int = analogRead(push_int);
  int button_out = analogRead(push_out);
  if(button_int>350)
    data_input=1;
  else if(button_out>350)
    data_output=1;
  else
  {
    data_input=0;
    data_output=0;
  }
  if(data_output==1&&data_input==0)
  {
    output_ok=1;  
    if(input_ok==1)
      data=2; 
  }
  else if(data_output==0&&data_input==1)
  {
    input_ok=1;
    if(output_ok==1)
      data=1;
  }
  else if(data_output==0&&data_input==0)
  {
    data=0;
    if(millis()-timer>10000)
    {
      timer=millis();
      output_ok=0; 
      input_ok=0;
    }
  }
  else
  {
    if(input_ok==1&&data_output==1)
    {
      data=2;
      input_ok=0;
    }
    else if(output_ok==1&&data_input==1)
    {
      data=1;
      output_ok=0;
    }

  }
  if(data>0)
  {
    output_ok=0;
    input_ok=0;
    if(data==1)  
    {
      music(1);
      data=0;
    }
    else if(data==2)  
      music(2);
    data=0;
  }
//  Serial.print(output_ok);
//  Serial.println(input_ok);
//   delay(1000);
}


