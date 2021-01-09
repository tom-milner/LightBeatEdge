//
// Created by Tom Milner on 22/12/2020.
//

#ifndef LIGHTBEATEDGE_GATEWAYCONSTANTS_H
#define LIGHTBEATEDGE_GATEWAYCONSTANTS_H


#define NUM_MESSAGE_TYPES 3

namespace GatewayConstants {
  namespace Messages {
    enum  MessageType {
      BEAT,
      NEW_MEDIA,
      MEDIA_FEATURES
    };
  }
  // This is altered upon setup to make it unique.
  static const char *EdgeIDBase = "LightBeatEdge-";

}// namespace BaseConstants

#endif//LIGHTBEATEDGE_GATEWAYCONSTANTS_H
