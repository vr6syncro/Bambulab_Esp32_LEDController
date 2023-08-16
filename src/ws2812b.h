#ifndef WS2812B_H
#define WS2812B_H

#include <FastLED.h>

#define DATA_PIN 16
#define NUM_LEDS 6
CRGB leds[NUM_LEDS];
int brightness = 84;

void initFastLED() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);  // should prevent the 5V Led Strip from drawing more than 500mA current to protect the USB port.
}

void setFastLEDBrightness(int new_brightness) {
  brightness = new_brightness;
  FastLED.setBrightness(brightness);
    preferences.begin("mqtt_config", false);
    preferences.putInt("brightness", brightness);
    preferences.end();
}


#endif