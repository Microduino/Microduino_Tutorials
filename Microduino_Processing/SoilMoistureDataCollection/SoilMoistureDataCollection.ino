int potPin = 4;//Set analog port A4 as the sensor signal input port
int val=0;
void setup()
{
  Serial.begin(9600);
}
void loop ()
{  
  val=analogRead(A4);//val value read from potPin signal port
  //Serial.print("Moisture Sensor Value:");  
  Serial.println(val);       
  delay(100);//delay
  }
