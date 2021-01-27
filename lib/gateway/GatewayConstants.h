//
// Created by Tom Milner on 22/12/2020.
//

#ifndef LIGHTBEATEDGE_GATEWAYCONSTANTS_H
#define LIGHTBEATEDGE_GATEWAYCONSTANTS_H


#define NUM_MESSAGE_TYPES 4

namespace GatewayConstants {
  namespace Messages {
    enum MessageType {
      TRIGGER,
      NEW_MEDIA,
      MEDIA_FEATURES,
      NEW_DEVICE
    };
  }
  // This is altered upon setup to make it unique.
  static const char *EdgeIDBase = "LightBeatEdge-";

}// namespace GatewayConstants

#endif//LIGHTBEATEDGE_GATEWAYCONSTANTS_H
