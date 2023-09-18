#ifndef LEDHANDLE_H
#define LEDHANDLE_H

#include "effects.h"

//include ledhandler for different cover
#include "ledhandle_icon_case.h"
#include "ledhandle_logo_case.h"
#include "ledhandle_stripe_case.h"


// Function to control LEDs based on the selected scenario - 1 is Icon Cover and 2 is Logo Cover and 3 is Strip Mode
void ledControlSwitch() {
  if (szenario == 1) {
    ledControlIconFastLED();
  } else if (szenario == 2) {
    ledControlLogoFastLED();
  } else if (szenario == 3) {
    ledControlStripeFastLED();
  }
  
}

//All LED off
void setFastLedAllBlack() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

// If Wifi is connecting do...
void setWifiConnecting() {
  if (szenario == 1) {
    leds[3] = CRGB::Orange;
    FastLED.show();
  } else if (szenario == 2) {
    // Handle "logo" scenario
  } else if (szenario == 3) {
    // Handle "Strip" scenario
  }
}

// If MQTT is connected do...
void setMqttConnected() {
  if (szenario == 1) {
    leds[3] = CRGB::Green;
    FastLED.show();
  } else if (szenario == 2) {
    // Handle "logo" scenario
  } else if (szenario == 3) {
    // Handle "Strip" scenario
  }
}

// If MQTT is disconnected do...
void setMqttDisconnected() {
  if (szenario == 1) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    FastLED.show();
  } else if (szenario == 2) {
    // Handle "logo" scenario
  } else if (szenario == 3) {
    // Handle "Strip" scenario
  }
}

#endif