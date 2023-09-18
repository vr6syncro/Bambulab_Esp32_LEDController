#ifndef EFFECTS_H
#define EFFECTS_H

#include <FastLED.h>

extern int NUM_LEDS;


unsigned long previousMillis_blink = 0;
bool ledState = false;

unsigned long previousMillis_fade = 0;
uint8_t fadeBrightness = 0;
bool isFadingIn = true;

unsigned long previousMillis_rainbow = 0;
uint8_t hueOffset = 0;

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
      if (fadeBrightness < brightness) {
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
    leds[ledIndex].fadeToBlackBy(map(fadeBrightness, 0, brightness, 255, 0));
    FastLED.show();
  }
}

// sample:     fadeInFadeOutAllLedsNonBlocking(10, CRGB::Red);
void fadeInFadeOutAllLedsNonBlocking(CRGB color, int fadeDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_fade >= fadeDelay) {
    previousMillis_fade = currentMillis;

    if (isFadingIn) {
      if (fadeBrightness < brightness) {
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

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
      leds[i].fadeToBlackBy(map(fadeBrightness, 0, brightness, 255, 0));
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
      leds[i] = CHSV((i * 256 / NUM_LEDS) + hueOffset, 255, 255);
    }
    FastLED.show();

    hueOffset++;
  }
}

void movingRainbowEffectNonBlockingForThreeLEDs(int rainbowDelay = 10) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_rainbow >= rainbowDelay) {
    previousMillis_rainbow = currentMillis;

    for (int i = 0; i < 3; i++) {
      leds[i] = CHSV((i * 256 / NUM_LEDS) + hueOffset, 255, 255);
    }
    FastLED.show();

    hueOffset++;
  }
}


#endif
