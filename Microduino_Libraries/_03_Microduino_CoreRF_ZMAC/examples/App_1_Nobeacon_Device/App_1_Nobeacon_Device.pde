#include <ZMAC.h>
#include "printf.h"

extern "C" int example_main(void);

void setup(void) 
{
  Serial.begin(57600);
  printf_begin();
  printf("AVR2025/examples/App_1_Nobeacon_Device/");
  example_main(); 
}
void loop(void) 
{

}

// vim:ai:cin:sw=2 sts=2 ft=cpp
