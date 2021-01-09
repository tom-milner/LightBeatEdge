//
// Created by Tom Milner on 07/01/2021.
//

#ifndef LIGHTBEATEDGE_LIGHTS_H
#define LIGHTBEATEDGE_LIGHTS_H

#include <FastLED.h>

class Lights {
  public:
  void flash(int hue, int duration);
  void setAllHue(int hue);
  void fadeByHueDelta(int hueDelta, int duration);
  void init(int nLeds);

  private:
  int numLeds;
  CRGB *leds;
  volatile int currentHue;
  int ringAdd(int a, int b, int max);
};


#endif//LIGHTBEATEDGE_LIGHTS_H
