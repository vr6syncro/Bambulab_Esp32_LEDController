#ifndef LEDHANDLE_ICON_CASE_H
#define LEDHANDLE_ICON_CASE_H

#include <FastLED.h>


extern bool hmsErrorExists;
extern int printer_stage;
extern bool f_layerInspection;
extern String gcodeState;
extern int NUM_LEDS;

void ledControlIconFastLED() {
  if (!f_layerInspection && hmsErrorExists) {
    leds[5] = CRGB::Red;
  } else if (f_layerInspection && hmsErrorExists) {
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Green;
    leds[4] = CRGB::Blue;
    leds[5] = CRGB::White;
  } else {
    leds[5] = CRGB::White;
  }

  if (gcodeState == "IDLE" || gcodeState == "RUNNING") {
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
  } else if (gcodeState == "PAUSE") {
      leds[0] = CRGB::Yellow;
      leds[1] = CRGB::Yellow;
      leds[2] = CRGB::Yellow;
      leds[4] = CRGB::White;
      leds[5] = CRGB::White;
  } else if (gcodeState == "FINISH") {
  if (finishTime == 0) {
    finishTime = millis();
  }
  if (millis() - finishTime < finishDisplayDuration) {
    movingRainbowEffectNonBlockingForThreeLEDs(10);
  } else {
    leds[0] = CRGB::White;
    leds[1] = CRGB::White;
    leds[2] = CRGB::White;
    leds[4] = CRGB::White;
    finishProcessed = true;
    }
  } else if (gcodeState != "FINISH") {
    finishTime = 0;
    finishProcessed = false;
  } else {
      leds[0] = CRGB::Purple;
      leds[1] = CRGB::Purple;
      leds[2] = CRGB::Purple;
      leds[4] = CRGB::White;
  }

  FastLED.show();
}

#endif