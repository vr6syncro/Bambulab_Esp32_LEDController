#ifndef LEDHANDLE_STRIPE_CASE_H
#define LEDHANDLE_STRIPE_CASE_H

#include <FastLED.h>


extern bool hmsErrorExists;
extern int printer_stage;
extern bool f_layerInspection;
extern String gcodeState;
extern int NUM_LEDS;

void ledControlStripeFastLED() {
  if (!f_layerInspection && hmsErrorExists) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
  } else if (f_layerInspection && hmsErrorExists) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  } else {

  }

  if (gcodeState == "IDLE" || gcodeState == "RUNNING") {
    switch (printer_stage) {
    case -1:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      break;
    case 0:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      break;
    case 11:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 1:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 2:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      break;
    case 4:
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      break;
    case 9:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 13:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 15:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 7:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 10:  // Printer Leveling
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 5:  // Printer M400 Pause
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      break;
    case 6:  // Printer Filament Runout
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
      break;
    case 8:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 18:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 19:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 12:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 14:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 3:  // Printer Calibrating
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case 16:  // Printer Paused by User
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      break;
    case 17:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    case 20:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    case 21:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    default:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      break;
  }
  } else if (gcodeState == "PAUSE") {
      // Insert the LED control code for the PAUSE state here
      // Example:
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
  } else if (gcodeState == "FINISH") {
  if (finishTime == 0) {
    finishTime = millis();
  }
  if (millis() - finishTime < finishDisplayDuration) {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::White);
    finishProcessed = true;
    }
  } else if (gcodeState != "FINISH") {
    finishTime = 0;
    finishProcessed = false;
  } else {
      fill_solid(leds, NUM_LEDS, CRGB::Purple);
  }

  FastLED.show();
}

#endif