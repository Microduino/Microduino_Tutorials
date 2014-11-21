#ifndef __ELF__
#define __ELF__
#include "room.h"

#define FACING_DOWN  0
#define FACING_UP    3
#define FACING_LEFT  6
#define FACING_RIGHT 9

#define STEP_LENGTH  4

#define MAX_ITEMS 3

#define ELFSTATE_PLAYING 0
#define ELFSTATE_DEAD 1
#define ELFSTATE_WON 2

struct Elf
{
  unsigned char facing;
  unsigned char step;
  unsigned char x;
  unsigned char y;
  unsigned char hearts;
  unsigned char items[4];
  unsigned char state;
};

void resetElf(bool reset_items);
void showElf();
void moveElf(unsigned char facing);
void throwSword();
RoomElement hitElf(RoomElement element);
Elf getElf();
void addElfItem(char type);
bool elfHasItem(char type);
char getElfState();

#endif __ELF__
