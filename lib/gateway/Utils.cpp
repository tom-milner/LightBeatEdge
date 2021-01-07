//
// Created by Tom Milner on 07/01/2021.
//

#include "Utils.h"


char Utils::getAlphaNumeric() {
  int idx = random(36);
  if (idx < 10) {
    return (char) idx + 48;
  }
  return (char) idx + 55;
}