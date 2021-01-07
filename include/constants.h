//
// Created by Tom Milner on 22/12/2020.
//

#ifndef LIGHTBEATEDGE_CONSTANTS_H
#define LIGHTBEATEDGE_CONSTANTS_H
namespace Constants {
  namespace MQTT {
    namespace Broker {
      const char *address = "192.168.1.129";
      const unsigned int port = 1883;
    }// namespace Broker
    namespace Topics {
      const char *beat = "beat";
      const char *newMedia = "new-media";
      const char *mediaFeatures = "media-features";
    }// namespace Topics

    // This is altered upon setup to make it unique.
    const char *ClientIDBase = "LightBeatEdge-";
  }// namespace MQTT
}// namespace Constants

#endif//LIGHTBEATEDGE_CONSTANTS_H
