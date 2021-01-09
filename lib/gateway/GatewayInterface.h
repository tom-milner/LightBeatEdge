//
// Created by Tom Milner on 08/01/2021.
//

#ifndef LIGHTBEATEDGE_GATEWAYINTERFACE_H
#define LIGHTBEATEDGE_GATEWAYINTERFACE_H

#include "BaseConstants.h"
#include <Arduino.h>

enum GatewayType {
  MQTT
};

class Gateway {
  public:
  virtual void init() = 0;
  virtual void onReceive(BaseConstants::Messages::MessageType, void (*handler)(char *topic, byte *payload, unsigned int length)) = 0;
  virtual void reconnect() = 0;
  virtual boolean isConnected() = 0;
  virtual ~Gateway() = default;
};
#endif//LIGHTBEATEDGE_GATEWAYINTERFACE_H
