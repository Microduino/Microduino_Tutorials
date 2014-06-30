/*

Run this sketch using a Zigduino and monitor the serial output
it will output comma separated data values when
you trigger it by sending something to the serial port

The comma separated format looks like
channel #, frequency, ED Level, RSSI

dump the data into a spreadsheet program and plot it out

try turning on other 2.4 GHz devices and see if the data changes

note that the valid channels for the PLL are from 11 to 26

*/

#include <ZigduinoRadio.h>

void setup()
{
  ZigduinoRadio.begin(11);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  
  Serial.println(); // signal the start with a new line
}

void loop()
{
  if (Serial.available()) {
    Serial.flush();
    
    char i;
    for (i = 11; i <= 26; i++) {
      ZigduinoRadio.setChannel(i);
      Serial.print(i, 10);
      Serial.print(",");
      Serial.print(((int16_t)(i*5))+2350, 10);
      Serial.print(",");
      Serial.print(ZigduinoRadio.getEdNow(), 10);
      Serial.print(",");
      Serial.print(ZigduinoRadio.getRssiNow(), 10);
      Serial.println(",");
    }
  }
}

void errHandle(radio_error_t err)
{
  Serial.println();
  Serial.print("Error: ");
  Serial.print((uint8_t)err, 10);
  Serial.println();
}