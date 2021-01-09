//
// Created by Tom Milner on 08/01/2021.
//

#ifndef LIGHTBEATEDGE_IGATEWAY_H
#define LIGHTBEATEDGE_IGATEWAY_H

#include "GatewayConstants.h"
#include <Arduino.h>

enum GatewayType {
  MQTT
};

class IGateway {
  public:
  virtual void init() = 0;
  virtual void onReceive(GatewayConstants::Messages::MessageType, void (*handler)(char *topic, byte *payload, unsigned int length)) = 0;
  virtual void reconnect() = 0;
  virtual boolean isConnected() = 0;
  virtual ~IGateway() = default;
};
#endif//LIGHTBEATEDGE_IGATEWAY_H
