#define PinA 2  
#define PinB 3 
#define PinC 6 

unsigned long time = 0; 
long count = 0; 

long num = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(PinA,INPUT_PULLUP);
  pinMode(PinB,INPUT_PULLUP);
  pinMode(PinC,INPUT_PULLUP);

  attachInterrupt(0, blinkA, LOW); 
  attachInterrupt(1, blinkB, LOW); 

  time = millis();
}

void loop()
{
  while (num != count)
  {
    num = count;
    Serial.println(num);
  }

  while ((count != 0) && (digitalRead(PinC) == LOW))
  {
    count = 0;
    num = count;
    Serial.println(num);
  }
}

void blinkA()
{
  if ((millis() - time) > 3) 
    count ++;
  time = millis();
}

void blinkB()
{
  if ((millis() - time) > 3) 
    count --;
  time = millis();
}




