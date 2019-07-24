#include <Arduboy2.h>
#include "avr/pgmspace.h"
#include <TimerOne.h>

// Based on HP35 emulator http://home.citycable.ch/pierrefleur/Jacques-Laporte/HP35_Arduino.htm
#include "rom.h"
#include "registers.h"

Arduboy2 arduboy;


#define SSIZE 12
#define WSIZE 14

int ledPin = RED_LED;

int i, tm;
int io_count = 2;
boolean display_enable = true;
boolean update_display = true;

/*LCD4Bit lcd = LCD4Bit(2);
  const int DataPin = 5;
  const int IRQpin =  3;
  PS2Keyboard_1 keyboard;*/
