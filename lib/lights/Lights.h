//
// Created by Tom Milner on 07/01/2021.
//

#ifndef LIGHTBEATEDGE_LIGHTS_H
#define LIGHTBEATEDGE_LIGHTS_H

#include <FastLED.h>

class Lights {
  public:
  void flash(int hue, int duration);
  void init(int nLeds);

  private:
  int numLeds;
  CRGB *leds;
};


#endif//LIGHTBEATEDGE_LIGHTS_H
