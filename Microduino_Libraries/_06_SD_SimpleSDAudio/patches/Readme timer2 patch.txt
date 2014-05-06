Replace wiring.c in Arduino V.1.0.1 in folder hardware/arduino/cores/arduino/ with the version in this folder to switch core timer functions from timer0 to timer2. 

This is usefull for ATmega328-Arduinos (Uno, Duemillanove, ..) to get four PWM outputs for stereo 16-bit playback. You can disable the timer-switch by commenting out the line "#define FREE_TIMER0 1" in wiring.c. 