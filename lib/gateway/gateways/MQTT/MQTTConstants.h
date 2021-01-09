//
// Created by Tom Milner on 08/01/2021.
//

#ifndef LIGHTBEATEDGE_MQTTCONSTANTS_H
#define LIGHTBEATEDGE_MQTTCONSTANTS_H

#include "GatewayConstants.h"

namespace MQTTConstants {
  namespace Broker {
    // TODO: Have these configured dynamically
    //  - config file?
    //  - void pointer to config struct.
    static const char *Address = "192.168.1.137";
    static const unsigned int Port = 1883;
  }// namespace Broker


  // Map the message types to topics.
  static char *Topics[NUM_MESSAGE_TYPES] = {
          [GatewayConstants::Messages::BEAT] = "beat",
          [GatewayConstants::Messages::NEW_MEDIA] = "new-media",
          [GatewayConstants::Messages::MEDIA_FEATURES] = "media-features"};


}// namespace MQTTConstants
#endif//LIGHTBEATEDGE_MQTTCONSTANTS_H
