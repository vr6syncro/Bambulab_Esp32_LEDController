#ifndef LEDHANDLE_ICON_CASE_H
#define LEDHANDLE_ICON_CASE_H

#include <FastLED.h>


//Deklariere welche Variablen genutzt werden sollen
extern bool hmsErrorExists;
extern int printer_stage;
extern bool f_layerInspection;

void ledControlIconFastLED() {
  /*
  if (debug) {
    Serial.println("Entering Icon Led Handler");
  }
  */

  if (f_layerInspection && hmsErrorExists) {
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Green;
    leds[4] = CRGB::Blue;
  } else if (hmsErrorExists) {
    leds[5] = CRGB::Red;
  } else {
    leds[5] = CRGB::White;
  }

  switch (printer_stage) {
    case -1:
      leds[0] = CRGB::White;
      leds[1] = CRGB::White;
      leds[2] = CRGB::White;
      leds[4] = CRGB::White;
      break;
    case 0:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 11:
      leds[0] = CRGB::Blue;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 1:
      leds[0] = CRGB::Blue;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 2:
      leds[0] = CRGB::Orange;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 4:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Yellow;
      leds[4] = CRGB::White;
      break;
    case 9:
      leds[0] = CRGB::Blue;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      leds[5] = CRGB::Green;
      break;
    case 13:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Blue;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      leds[5] = CRGB::Green;
      break;
    case 15:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Orange;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 7:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Orange;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 10:  // Printer Leveling
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 5:  // Printer M400 Pause
      leds[0] = CRGB::Yellow;
      leds[1] = CRGB::Yellow;
      leds[2] = CRGB::Yellow;
      leds[4] = CRGB::White;
      break;
    case 6:  // Printer Filament Runout
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Red;
      leds[4] = CRGB::White;
      break;
    case 8:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 18:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 19:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 12:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 14:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Blue;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 3:  // Printer Calibrating
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::Blue;
      break;
    case 16:  // Printer Paused by User
      leds[0] = CRGB::Yellow;
      leds[1] = CRGB::Yellow;
      leds[2] = CRGB::Yellow;
      leds[4] = CRGB::White;
      break;
    case 17:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 20:
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Green;
      leds[4] = CRGB::White;
      break;
    case 21:
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Red;
      leds[4] = CRGB::White;
      break;
    default:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[4] = CRGB::Black;
      leds[5] = CRGB::Black;
      break;
  }

  FastLED.show();
}

#endif