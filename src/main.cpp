#include <Arduino.h>

#include "constants.h"
#include "credentials.h"

#include <PubSubClient.h>


#if ESP32
#define LED_BUILTIN 4
#include <WiFi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#endif

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


// The number of bytes to add to the base id to make the client id unique.
const char numIDBytes = 4;
char *mqttClientID;

char ledState = 0;


// This function is called whenever we receive a message via mqtt.
void messageCallback(char *topic, byte *payload, unsigned int length) {
   Serial.print(topic);
   Serial.print(": ");
   for (int i = 0; i < length; i++) {
   Serial.print((char) payload[i]);
   }
  ledState ^= 1;
  digitalWrite(LED_BUILTIN, ledState);

   Serial.println();
}

char getAlphaNumberic() {
  int idx = random(36);
  if (idx < 10) {
    return (char) idx + 48;
  }
  return (char) idx + 55;
}


void generateClientID() {
  const int baseIDLength = strlen(Constants::MQTT::ClientIDBase);
  mqttClientID = (char *) malloc(baseIDLength + numIDBytes + 1);// +1 for \0
  memset(mqttClientID, '\0', sizeof(mqttClientID));
  strcpy(mqttClientID, Constants::MQTT::ClientIDBase);

  // Add the random bytes to the id string.
  for (int i = 0; i < numIDBytes; i++) {
    char c = getAlphaNumberic();
    strncat(mqttClientID, &c, 1);
  }
}


void setupWiFi() {
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
  WiFi.hostname(mqttClientID);
#else
  WiFi.setHostname(mqttClientID);
  WiFi.setSleep(false);
#endif

  // We are now connected!
  Serial.println();
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

//  Turn of BLE on esp32 (apparently this decreases WiFi latency).
#if ESP32
  btStop();
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
#endif

  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println();
  Serial.println("************************************");
  Serial.println("         LightBeatEdge v0.1         ");
  Serial.println("************************************");

  // Create our unique ClientID.
  generateClientID();
  Serial.print("Client ID: ");
  Serial.println(mqttClientID);

  // Connect to the network.
  setupWiFi();

  // Initialize MQTT client.
  mqttClient.setServer(Constants::MQTT::Broker::address, Constants::MQTT::Broker::port);
  mqttClient.setCallback(messageCallback);
}

// Reconnect to the MQTT broker.
void reconnectMQTT() {

  // Block until we're reconnected.
  while (!mqttClient.connected()) {

    // Attempt to connect.
    if (!mqttClient.connect(mqttClientID)) {
      // Connection failed.
      Serial.println("MQTT connection failed.");
      Serial.println(mqttClient.state());
      Serial.println("Trying again in 5 seconds.");
      delay(5000);
      continue;// Early exit.
    }

    // Connection successful.
    Serial.println("MQTT connected.");

    // Subscribe to the relevant topics.
    mqttClient.subscribe(Constants::MQTT::Topics::beat);
    mqttClient.loop();
    mqttClient.subscribe(Constants::MQTT::Topics::newMedia);
    mqttClient.loop();
    mqttClient.subscribe(Constants::MQTT::Topics::mediaFeatures);
    mqttClient.loop();

  }
}

void loop() {

  // Make sure we are connected to the MQTT broker.
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
}