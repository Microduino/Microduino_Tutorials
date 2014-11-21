#include <avr/pgmspace.h>
#include <TVout.h>
#include "display.h"
#include "room.h"

extern TVout TV;
unsigned int score;

void display_update(unsigned int add_to_score)
{
   score += add_to_score; 
   TV.print(0, 88, score);
}

void display_init()
{
   score = 0;
}
