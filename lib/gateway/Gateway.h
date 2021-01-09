//
// Created by Tom Milner on 08/01/2021.
//

#ifndef LIGHTBEATEDGE_GATEWAY_H
#define LIGHTBEATEDGE_GATEWAY_H


#include "GatewayInterface.h"
#include "gateways/MQTT/MQTTGateway.h"

Gateway * createGateway(GatewayType type){
  switch (type){
    case MQTT:
      return new MQTTGateway();
  }
}
#endif//LIGHTBEATEDGE_GATEWAY_H
