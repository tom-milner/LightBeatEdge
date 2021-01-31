//
// Created by Tom Milner on 08/01/2021.
//

#ifndef LIGHTBEATEDGE_MQTTCONSTANTS_H
#define LIGHTBEATEDGE_MQTTCONSTANTS_H

#include "GatewayConstants.h"
namespace MQTTConstants {
  // Map the message types to topics.
  static char *Topics[NUM_MESSAGE_TYPES] = {
          [GatewayConstants::Messages::TRIGGER] = "trigger",
          [GatewayConstants::Messages::NEW_MEDIA] = "new-media",
          [GatewayConstants::Messages::MEDIA_FEATURES] = "media-features",
          [GatewayConstants::Messages::NEW_DEVICE] = "new-device"
  };


}// namespace MQTTConstants
#endif//LIGHTBEATEDGE_MQTTCONSTANTS_H
