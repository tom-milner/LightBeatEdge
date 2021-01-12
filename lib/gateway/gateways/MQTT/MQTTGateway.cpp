//
// Created by Tom Milner on 07/01/2021.
//

#include "MQTTGateway.h"

// Initialise the gateway connection.
void MQTTGateway::init() {
  edgeID = generateEdgeID();
  setupWifi(edgeID);
  setupMQTT();
}

// Generate a unique identifier for this device.
char *MQTTGateway::generateEdgeID() {
  const int baseIDLength = strlen(GatewayConstants::EdgeIDBase);
  const char numIDBytes = 4;// The number of bytes to add to the base id to make the client id unique.

  char *edgeID = (char *) malloc(baseIDLength + numIDBytes + 1);// +1 for \0
  memset(edgeID, '\0', baseIDLength + numIDBytes + 1);
  strcpy(edgeID, GatewayConstants::EdgeIDBase);

  // Add the random bytes to the id string.
  for (int i = 0; i < numIDBytes; i++) {
    char c = Utils::getAlphaNumeric();
    strncat(edgeID, &c, 1);
  }
  return edgeID;
}


// Setup the MQT connection.
void MQTTGateway::setupMQTT() {
  Serial.println("Setting up MQTT...");
  mqttClient.setClient(wifiClient);
  mqttClient.setServer(MQTTConstants::Broker::Address, MQTTConstants::Broker::Port);
  Serial.println(" - Server set.");
  mqttClient.setCallback(messageReceiver);
  Serial.println(" - Callback set.");

  // Make all handlers do nothing (to start off with).
  for (int i = 0; i < NUM_MESSAGE_TYPES; i++) {
    handlers[i] = &doNothing;
  }
  Serial.println(" - Handler defaults set.");
}

// Setup the WiFi connection.
void MQTTGateway::setupWifi(char *hostname) {
  Serial.println();
  Serial.println("Connecting to: ");
  Serial.print("SSID: ");
  Serial.println(Credentials::WiFi::ssid);
  Serial.print("Pass: ");
  Serial.println(Credentials::WiFi::password);

  WiFi.mode(WIFI_STA);

#if ESP32
  WiFi.disconnect();
  // This line is required to get setHostname working properly.
  // WiFi.config(IPADDR_NONE, IPADDR_NONE, IPADDR_NONE, IPADDR_NONE);
  WiFi.setHostname(hostname);
  WiFi.setSleep(false);
#endif


  WiFi.begin(Credentials::WiFi::ssid, Credentials::WiFi::password);

  // Wait for connection.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

#if ESP8266
  WiFi.hostname(hostname);
#endif
  delay(150);
  // We are now connected!
  Serial.println();
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());
}

// Specify what function should be run when the given message type is received.
void MQTTGateway::onReceive(GatewayConstants::Messages::MessageType messageType, void (*handler)(char *topic, byte *payload, unsigned int length)) {
  // Override the default handler with the supplied handler.
  handlers[messageType] = handler;
}


// Reconnect to the gateway.
void MQTTGateway::reconnect() {

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
      Serial.print(mqttClient.subscribe(MQTTConstants::Topics[i]));
      mqttClient.loop();
      Serial.print(" - Subscribed to ");
      Serial.println(MQTTConstants::Topics[i]);
    }
  }
  mqttClient.loop();
}

// Whether or not we are connected to the gateway.
boolean MQTTGateway::isConnected() {

  mqttClient.loop();
  return mqttClient.connected();
}

// Call the relevant handler for the given topic.
void messageReceiver(char *topic, byte *payload, unsigned int length) {

  printMQTT(topic, payload, length);
  for (int i = 0; i < NUM_MESSAGE_TYPES; i++) {
    if (strcmp(topic, MQTTConstants::Topics[i]) == 0) {
      (handlers[i])(topic, payload, length);
      return;
    }
  }
}

// The default handler function. Does what it says on the tin!
void doNothing(char *topic, byte *payload, unsigned int length) {}

void printMQTT(char *topic, byte *payload, unsigned int length) {
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}