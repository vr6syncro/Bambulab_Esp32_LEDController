#ifndef EFFECTS_H
#define EFFECTS_H

#include <FastLED.h>


unsigned long previousMillis_blink = 0;  // Speichert den Zeitpunkt des letzten Blinkens
bool ledState = false;                   // Zustand der LED: true = eingeschaltet, false = ausgeschaltet

unsigned long previousMillis_fade = 0;
uint8_t fadeBrightness = 0;  // Aktuelle Helligkeit der LED für den Fade-Effekt
bool isFadingIn = true;      // Gibt an, ob die LED gerade heller wird

unsigned long previousMillis_rainbow = 0;
uint8_t hueOffset = 0;  // Dieser Wert wird mit der Zeit erhöht, um den Regenbogeneffekt zu verschieben

// sample:     blinkLEDNonBlocking(4, 1000, CRGB::Red);
void blinkLEDNonBlocking(int ledIndex, int blinkDelay = 500, CRGB color = CRGB::White) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_blink >= blinkDelay) {
    previousMillis_blink = currentMillis;

    if (ledState) {
      leds[ledIndex] = CRGB::Black;
    } else {
      leds[ledIndex] = color;
    }

    FastLED.show();
    ledState = !ledState;
  }
}

// sample:     fadeInFadeOutNonBlocking(4, 10, CRGB::Red);
void fadeInFadeOutNonBlocking(int ledIndex, CRGB color, int fadeDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_fade >= fadeDelay) {
    previousMillis_fade = currentMillis;

    if (isFadingIn) {
      if (fadeBrightness < brightness) {  // Hier verwenden wir die globale brightness-Variable
        fadeBrightness++;
      } else {
        isFadingIn = false;
      }
    } else {
      if (fadeBrightness > 0) {
        fadeBrightness--;
      } else {
        isFadingIn = true;
      }
    }

    leds[ledIndex] = color;
    leds[ledIndex].fadeToBlackBy(map(fadeBrightness, 0, brightness, 255, 0));  // Berücksichtigung der globalen brightness
    FastLED.show();
  }
}

// sample:     fadeInFadeOutAllLedsNonBlocking(10, CRGB::Red);
void fadeInFadeOutAllLedsNonBlocking(CRGB color, int fadeDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_fade >= fadeDelay) {
    previousMillis_fade = currentMillis;

    if (isFadingIn) {
      if (fadeBrightness < brightness) {  // Hier verwenden wir die globale brightness-Variable
        fadeBrightness++;
      } else {
        isFadingIn = false;
      }
    } else {
      if (fadeBrightness > 0) {
        fadeBrightness--;
      } else {
        isFadingIn = true;
      }
    }

    for (int i = 0; i < NUM_LEDS; i++) {  // Vorausgesetzt, Sie haben eine Konstante namens NUM_LEDS, die die Anzahl der LEDs angibt.
      leds[i] = color;
      leds[i].fadeToBlackBy(map(fadeBrightness, 0, brightness, 255, 0));  // Berücksichtigung der globalen brightness
    }
    FastLED.show();
  }
}


// sample:     movingRainbowEffectNonBlocking(10);
void movingRainbowEffectNonBlocking(int rainbowDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_rainbow >= rainbowDelay) {
    previousMillis_rainbow = currentMillis;

    for (int i = 0; i < NUM_LEDS; i++) {
      // Der Farbhue wird basierend auf der Position der LED und dem globalen Offset berechnet
      leds[i] = CHSV((i * 256 / NUM_LEDS) + hueOffset, 255, 255);
    }
    FastLED.show();

    hueOffset++;  // Erhöht den Farbhue-Offset, um den Regenbogeneffekt zu verschieben
  }
}

void movingRainbowEffectNonBlockingForThreeLEDs(int rainbowDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_rainbow >= rainbowDelay) {
    previousMillis_rainbow = currentMillis;

    for (int i = 0; i < 3; i++) {  // Nur für LEDs 0, 1 und 2
      // Der Farbhue wird basierend auf der Position der LED und dem globalen Offset berechnet
      leds[i] = CHSV((i * 256 / NUM_LEDS) + hueOffset, 255, 255);
    }
    FastLED.show();

    hueOffset++;  // Erhöht den Farbhue-Offset, um den Regenbogeneffekt zu verschieben
  }
}


#endif
