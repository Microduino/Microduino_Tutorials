#include "arduino.h"

int _V_bat=_V_min;
void vobat()
{
  _V_bat=10*((5.0f*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f)));
  if(_V_bat<_V_min)  _V_bat=_V_min;
  if(_V_bat>_V_max)  _V_bat=_V_max;

#ifdef DEBUG
  Serial.print("_V_bat: ");
  Serial.println(_V_bat);
#endif
}
