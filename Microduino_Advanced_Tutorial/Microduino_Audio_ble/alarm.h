volatile int alarm_flag=0;
boolean alarm_sta=false;
boolean alarm_switch;
int alarm_hour,alarm_minute;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

int alarm_tone;
int alarm_vol;

boolean test_audio=false;
boolean test_audio_vol=false;

void do_alarm(int _do_alarm)
{
  audio_choose(_do_alarm);
  audio_play();
}

void end_alarm(int _end_alarm)
{
  audio_pause();
}

void test_alarm(int _test_alarm)
{
  do_alarm(_test_alarm);
}

void test_alarm_vol(int _test_alarm_vol)
{
  audio_vol(_test_alarm_vol);
}

/* the interrupt service routine */
void blink()
{
  alarm_flag=1;
}

void set_alarm(int _alarm_hour,int _alarm_minute)
{
  pcf_rtc.clearAlarm();
  pcf_rtc.setAlarm(_alarm_minute, _alarm_hour, 99, 99);
  //Serial.println("debug set alarm");

  detachInterrupt(0);
  alarm_flag=0;
  /* setup int on pin 2 of arduino */
  attachInterrupt(0, blink, FALLING);
}
