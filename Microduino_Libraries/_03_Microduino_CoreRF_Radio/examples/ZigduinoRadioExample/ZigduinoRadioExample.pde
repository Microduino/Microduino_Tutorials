/*

Run this sketch on two Zigduinos, open the serial monitor at 9600 baud, and type in stuff
Watch the Rx Zigduino output what you've input into the serial port of the Tx Zigduino

*/

#include <ZigduinoRadio.h>

void setup()
{
  ZigduinoRadio.begin(11);
  Serial.begin(9600);
  
  ZigduinoRadio.attachError(errHandle);
  ZigduinoRadio.attachTxDone(onXmitDone);
}

void loop()
{
  if (Serial.available())
  {
    ZigduinoRadio.beginTransmission();
    
    Serial.println();
    Serial.print("Tx: ");
    
    while(Serial.available())
    {
      char c = Serial.read();
      Serial.write(c);
      ZigduinoRadio.write(c);
    }
    
    Serial.println(); 
    
    ZigduinoRadio.endTransmission();
  }
  
  if (ZigduinoRadio.available())
  {
    Serial.println();
    Serial.print("Rx: ");
    
    while(ZigduinoRadio.available())
      Serial.write(ZigduinoRadio.read());
      
    Serial.println();
    Serial.print("LQI: ");
    Serial.print(ZigduinoRadio.getLqi(), 10);
    Serial.print(", RSSI: ");
    Serial.print(ZigduinoRadio.getLastRssi(), 10);
    Serial.print(" dBm, ED: ");
    Serial.print(ZigduinoRadio.getLastEd(), 10);
    Serial.println("dBm");
  }
  
  delay(100);
}

void errHandle(radio_error_t err)
{
  Serial.println();
  Serial.print("Error: ");
  Serial.print((uint8_t)err, 10);
  Serial.println();
}

void onXmitDone(radio_tx_done_t x)
{
  Serial.println();
  Serial.print("TxDone: ");
  Serial.print((uint8_t)x, 10);
  Serial.println();
}