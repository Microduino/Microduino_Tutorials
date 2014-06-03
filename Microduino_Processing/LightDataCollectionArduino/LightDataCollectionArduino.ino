#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);

float sensor_lux=0.00;


void setup()
{
  Serial.begin(9600);
  tsl.begin();
}
void loop()
{
  sensors_event_t event;
  tsl.getEvent(&event);
  /* Display the results (light is measured in lux) */
  if (event.light) {
    sensor_lux=event.light;
    Serial.println(sensor_lux);
  }
  delay(100);
}


