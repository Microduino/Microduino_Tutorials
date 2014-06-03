#include <Wire.h>
#include <AM2321.h>
AM2321 am2321;

float temperature=0.0;
float humidity=0.0;

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  am2321.read();
  temperature=(float)am2321.temperature/10.0;
  humidity=(float)am2321.humidity/10.0;
  Serial.print(temperature);
  Serial.print(",");
  Serial.println(humidity);
  delay(100);
}


