//
// Created by Tom Milner on 07/01/2021.
//

#include "Lights.h"


// Turn the lights on for a given duration.
void Lights::flash(int hue, int duration) {
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
  unsigned long start = millis();
  while (millis() - start < duration) {}
  for (int i = 0; i < numLeds; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Setup the lights.
void Lights::init(int nLeds) {
  numLeds = nLeds;
  leds = (CRGB *) malloc(nLeds * sizeof(CRGB));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, nLeds);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1800);

}

// Change the color of the lights.
void Lights::setAllHue(int hue) {
  currentHue = hue;
  CHSV chsv;
  for (int i = 0; i < numLeds; i++) {
    hsv2rgb_spectrum(CHSV(hue, 255, 255), leds[i]);
  }
  FastLED.show();
}
void Lights::fadeByHueDelta(int hueDelta, int duration) {
  int incr = hueDelta > 0 ? 1 : -1;
  if (duration == 0 ){
    setAllHue(ringAdd(currentHue, hueDelta,255));
    return;
  }
  int durationPerHue = duration / hueDelta;
  int last = millis();
  for (int i = 0; i < hueDelta; i++) {
    while (millis() - last < durationPerHue) {}
    last = millis();
    setAllHue( ringAdd(currentHue, incr, 255));
  }
  Serial.println(currentHue);
}
int Lights::ringAdd(int a, int b, int max) {
  int out = a + b;
  if (out >= max) out -= max;
  if (out < 0) out += max;
  return out;
}
