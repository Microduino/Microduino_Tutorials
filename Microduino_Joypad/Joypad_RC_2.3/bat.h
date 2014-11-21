int _V_bat=_V_min;

void vobat()
{
  _V_bat=((analogRead(PIN_bat)/52.08f));  //_V_bat=10*((voltage*anglogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f)));
  if(_V_bat<_V_min) _V_bat=_V_min;
  if(_V_bat>_V_max) _V_bat=_V_max;

#ifdef Serial_DEBUG
  Serial.print("_V_bat: ");
  Serial.println(_V_bat);
#endif
}
