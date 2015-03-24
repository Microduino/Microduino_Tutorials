#include <dht11.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

dht11 DHT11;
#define DHT11PIN 6 //DHT11 PIN 3 


String humidity,temperature;
String tempHumi;

char buff[10];
 
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}
 
void loop()
{
  Serial.println("\n");
 
  int chk = DHT11.read(DHT11PIN);
 
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature-2, 2);


  dtostrf((float)DHT11.temperature, 6, 2, buff); 
  temperature=buff;
  temperature.trim();

  dtostrf((float)DHT11.humidity, 6, 2, buff); 
  humidity=buff;
  humidity.trim();
  //temperature=String((float)DHT11.temperature);
  //humidity=String((float)DHT11.humidity);
  tempHumi=temperature+','+humidity;

  mySerial.println(tempHumi);

  delay(2000);
}
