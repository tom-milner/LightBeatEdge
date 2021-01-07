//
// Created by Tom Milner on 07/01/2021.
//

#ifndef LIGHTBEATEDGE_GATEWAY_H
#define LIGHTBEATEDGE_GATEWAY_H

#include <PubSubClient.h>

#if ESP32
#include <WiFi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif

#include "constants.h"
#include "credentials.h"

#include "Utils.h"

enum GatewayType {
  MQTT
};

class Gateway {

  public:
  void init(GatewayType type);
  void onReceive(Constants::MessageType, void (*handler)(char *topic, byte *payload, unsigned int length));
  void reconnect();
  boolean isConnected();


  private:
  // TODO: Make gateway an interface, and move all MQTT and WiFI stuff into seperate file.
  void setupWifi(char *hostname);
  void setupMQTT();
  char *generateEdgeID();
  GatewayType gatewayType;
  WiFiClient wifiClient;
  PubSubClient mqttClient;
  char *edgeID;
};
static void (*handlers[NUM_MESSAGE_TYPES])(char *topic, byte *payload, unsigned int length);
void doNothing(char *topic, byte *payload, unsigned int length);
void messageReceiver(char *topic, byte *payload, unsigned int length);

#endif//LIGHTBEATEDGE_GATEWAY_H
