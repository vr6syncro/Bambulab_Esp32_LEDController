#ifndef WS2812B_H
#define WS2812B_H

#include <FastLED.h>

extern Preferences preferences;
extern int szenario;

#define DATA_PIN 16
#define NUM_LEDS_FOR_SCENARIO_1_2 6  // Anzahl der LEDs für Szenarien 1 und 2
int led_scenario_3 = 1;  // Anzahl der LEDs für Szenario 3 (kann geändert werden)

// Globale Variable für die aktuelle Anzahl der LEDs
int NUM_LEDS;

CRGB *leds;  // Dynamisches Array für LEDs
int brightness = 84;

void initFastLED() {
  if (szenario == 3) {
    NUM_LEDS = led_scenario_3;
    FastLED.setMaxPowerInVoltsAndMilliamps(5, INT_MAX);  // Entfernen der Strombegrenzung für Szenario 3
  } else {
    NUM_LEDS = NUM_LEDS_FOR_SCENARIO_1_2;
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);  // Standardmäßige Strombegrenzung für Szenarien 1 und 2
  }

  // Initialisieren der LEDs
  leds = new CRGB[NUM_LEDS];
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void setFastLEDBrightness(int new_brightness) {
  brightness = new_brightness;
  FastLED.setBrightness(brightness);
  preferences.begin("mqtt_config", false);
  preferences.putInt("brightness", brightness);
  preferences.end();
}

#endif