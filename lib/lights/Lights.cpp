//
// Created by Tom Milner on 07/01/2021.
//

#include "Lights.h"


// Turn the lights on for a given duration.
void Lights::flash(int hue, int duration) {
  for (int i = 0 ; i < numLeds; i++){
    leds[i] = CHSV(hue, 255,255);
  }
  FastLED.show();
  unsigned long start = millis();
  while(millis() - start < duration){}
  for (int i = 0 ; i < numLeds; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();

}

// Setup the lights.
void Lights::init(int nLeds) {
  numLeds = nLeds;
  leds = (CRGB *) malloc(nLeds * sizeof(CRGB));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, nLeds);
}

// Change the color of the lights.
void Lights::setAllHue(int hue) {
  for(int i = 0 ; i < numLeds; i++){
    leds[i] = CHSV(hue, 255,255);
  }
  FastLED.show();
}

