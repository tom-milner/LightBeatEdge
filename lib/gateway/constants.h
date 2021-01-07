//
// Created by Tom Milner on 22/12/2020.
//

#ifndef LIGHTBEATEDGE_CONSTANTS_H
#define LIGHTBEATEDGE_CONSTANTS_H


#define NUM_MESSAGE_TYPES 3

namespace Constants {
   enum MessageType {
    BEAT,
    NEW_MEDIA,
    MEDIA_FEATURES
  };

  namespace MQTT {
    namespace Broker {
     static const char *address = "192.168.1.137";
     static const unsigned int port = 1883;
    }// namespace Broker

    static char *Topics[NUM_MESSAGE_TYPES] = {
            "beat",
            "new-media",
            "media-features"};// namespace MessageType

  }// namespace MQTT

  // This is altered upon setup to make it unique.
  static const char *EdgeIDBase = "LightBeatEdge-";

}// namespace Constants

#endif//LIGHTBEATEDGE_CONSTANTS_H
