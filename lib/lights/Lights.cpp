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

void Lights::fadeToHue(int hue, int duration) {
  if (hue == currentHue) return;

  int clockwiseDelta = hue - currentHue;
  Serial.printf("Clockwise: %i\n", clockwiseDelta);

  int anticlockwiseDelta = 256 - abs(clockwiseDelta);
  Serial.printf("Anti-Clockwise: %i\n", anticlockwiseDelta);
  int hueDelta;
  if (clockwiseDelta < anticlockwiseDelta) {
    hueDelta = clockwiseDelta;
  }else {
    hueDelta = anticlockwiseDelta * -1;
  }

  Serial.print("Hue Delta: ");
  Serial.println(hueDelta);
  fadeByHueDelta(hueDelta, duration);
}
void Lights::fadeByHueDelta(int hueDelta, int duration) {
  if (hueDelta == 0) return;
  int incr = hueDelta > 0 ? 1 : -1;
  if(incr == -1 ) hueDelta *= -1;
  if (duration == 0) {
    setAllHue(ringAdd(currentHue, hueDelta, 256));
    return;
  }
  int durationPerHue = duration / hueDelta;
  int last = millis();
  for (int i = 0; i < abs(hueDelta); i++) {
    while (millis() - last < durationPerHue) {}
    last = millis();
    setAllHue(ringAdd(currentHue, incr, 256));
  }
  Serial.println(currentHue);
}
int Lights::ringAdd(int a, int b, int circ) {
  int out = a + b;
  if (out >= circ) out -= circ;
  if (out < 0) out += circ;
  return out;
}
