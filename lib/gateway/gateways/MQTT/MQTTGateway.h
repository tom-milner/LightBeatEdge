//
// Created by Tom Milner on 07/01/2021.
//

#ifndef LIGHTBEATEDGE_MQTTGATEWAY_H
#define LIGHTBEATEDGE_MQTTGATEWAY_H

#include <PubSubClient.h>

#if ESP32
#include <WiFi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif

#include "Credentials.h"
#include "MQTTConstants.h"

#include "GatewayInterface.h"
#include "Utils.h"


class MQTTGateway : public Gateway {

  public:
  void init() override;
  void onReceive(BaseConstants::Messages::MessageType, void (*handler)(char *topic, byte *payload, unsigned int length)) override;
  void reconnect() override;
  boolean isConnected() override;


  private:
  void setupWifi(char *hostname);
  void setupMQTT();
  char *generateEdgeID();
  WiFiClient wifiClient;
  PubSubClient mqttClient;
  char *edgeID;
};
static void (*handlers[NUM_MESSAGE_TYPES])(char *topic, byte *payload, unsigned int length);
void doNothing(char *topic, byte *payload, unsigned int length);
void messageReceiver(char *topic, byte *payload, unsigned int length);

#endif//LIGHTBEATEDGE_MQTTGATEWAY_H
