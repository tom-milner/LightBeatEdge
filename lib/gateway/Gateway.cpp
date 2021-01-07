//
// Created by Tom Milner on 07/01/2021.
//

#include "Gateway.h"

void Gateway::init(GatewayType type) {
  gatewayType = type;
  switch (gatewayType) {
    case MQTT:
      edgeID = generateEdgeID();
      setupWifi(edgeID);
      setupMQTT();
      break;
  }
}


char *Gateway::generateEdgeID() {
  const int baseIDLength = strlen(Constants::EdgeIDBase);
  const char numIDBytes = 4;// The number of bytes to add to the base id to make the client id unique.

  char *edgeID = (char *) malloc(baseIDLength + numIDBytes + 1);// +1 for \0
  memset(edgeID, '\0', baseIDLength + numIDBytes + 1);
  strcpy(edgeID, Constants::EdgeIDBase);

  // Add the random bytes to the id string.
  for (int i = 0; i < numIDBytes; i++) {
    char c = Utils::getAlphaNumeric();
    strncat(edgeID, &c, 1);
  }
  return edgeID;
}

void Gateway::setupMQTT() {
  Serial.println("Setting up MQTT...");
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(Constants::MQTT::Broker::address, Constants::MQTT::Broker::port);
  Serial.println(" - Server set.");
  mqttClient.setCallback(messageReceiver);
  Serial.println(" - Callback set.");

  // Make all handlers do nothing (to start off with).
  for (int i = 0; i < NUM_MESSAGE_TYPES; i++) {
    handlers[i] = &doNothing;
  }
  Serial.println(" - Handler defaults set.");


}

void Gateway::setupWifi(char *hostname) {
  Serial.println();
  Serial.println("Connecting to: ");
  Serial.print("SSID: ");
  Serial.println(Credentials::WiFi::ssid);
  Serial.print("Pass: ");
  Serial.println(Credentials::WiFi::password);

  WiFi.mode(WIFI_STA);
  WiFi.begin(Credentials::WiFi::ssid, Credentials::WiFi::password);

  // Wait for connection.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

#if ESP8266
  WiFi.hostname(hostname);
#else
  WiFi.setHostname(hostname);
  WiFi.setSleep(false);
#endif

  // We are now connected!
  Serial.println();
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}



void Gateway::onReceive(Constants::MessageType messageType, void (*handler)(char *topic, byte *payload, unsigned int length)) {
  // Override the default handler with the supplied handler.
  handlers[messageType] = handler;
}
void Gateway::reconnect() {
  switch (gatewayType) {
    case MQTT:
      // Block until we're reconnected.
      while (!mqttClient.connected()) {

        // Attempt to connect.
        if (!mqttClient.connect(edgeID)) {
          // Connection failed.
          Serial.println("MQTT connection failed.");
          Serial.println(mqttClient.state());
          Serial.println("Trying again in 5 seconds.");
          delay(5000);
          continue;// Early exit.
        }
        // Connection successful.
        Serial.println("MQTT connected.");

        // Subscribe to all the topics.
        for (int i = 0; i < NUM_MESSAGE_TYPES; i++) {
          Serial.println(mqttClient.subscribe(Constants::MQTT::Topics[i]));
          mqttClient.loop();
          Serial.print(" - Subscribed to ");
          Serial.println(Constants::MQTT::Topics[i]);
        }
      }
            mqttClient.loop();
      break;
  }
}
boolean Gateway::isConnected() {
  switch (gatewayType) {
    case MQTT:
      mqttClient.loop();
      return mqttClient.connected();
  }
  return false;
}

void messageReceiver(char *topic, byte *payload, unsigned int length) {
  for (int i = 0; i < NUM_MESSAGE_TYPES; i++) {
    if (strcmp(topic, Constants::MQTT::Topics[i]) == 0) {
      (handlers[i])(topic, payload, length);
    }
  }
}

void doNothing(char *topic, byte *payload, unsigned int length) {}