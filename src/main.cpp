

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Gateway.h>
#include <Lights.h>

#include "Config.h"

IGateway *gateway;
Lights lights;

MQTTConfig config;

// The function to handle any incoming trigger messages.
void triggerHandler(byte *payload, unsigned int length) {
  StaticJsonDocument<40> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.println(error.c_str());
  }
  int duration = doc["duration"].as<int>();
  //  int number = doc["number"].as<int>();

  lights.fadeByHueDelta(32, duration / 4);
}

// The function to handle any incoming media-features messages.
void mediaFeaturesHandler( byte * payload, unsigned int length){
  StaticJsonDocument<JSON_OBJECT_SIZE(9)> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if(error){
    Serial.println(error.c_str());
  }
//  float valence = doc["valence"].as<float>();
  float energy = doc["energy"].as<float>();
  float tempo = doc["tempo"].as<float>();

  int newHue = lights.ringAdd(energy * 255, -85, 255);

  Serial.print("New Hue: ");
  Serial.println(newHue);
  lights.fadeToHue(newHue, 100);
}

// Print the application banner to serial.
void printBanner() {
  Serial.println();
  Serial.println();
  Serial.println("************************************");
  Serial.println("         LightBeatEdge v0.1         ");
  Serial.println("************************************");
}


void setup() {

//  Turn off BLE on esp32 (apparently this decreases WiFi latency).
#if ESP32
  btStop();
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Make serial accessible via pin 16 & 17
#endif

  Serial.begin(115200);
  delay(500);
  printBanner();

  // Move the config options into a struct usable by the gateway.
  BrokerInfo broker;
  broker.Address = Config::Broker::Address;
  broker.Port = Config::Broker::Port;
  WifiInfo wifi;
  wifi.SSID = Config::WiFi::SSID;
  wifi.Password = Config::WiFi::Password;
  config.Wifi = wifi;
  config.Broker = broker;


  // Setup the lights.
  lights.init(300);

  // Create gateway.
  gateway = createGateway(MQTT);
  gateway->init((void *) &config); // Config object must be passed to void to keep interface consistent across gateway types.

  // Setup message handlers.
  gateway->onReceive(GatewayConstants::Messages::TRIGGER, triggerHandler);
  gateway->onReceive(GatewayConstants::Messages::MEDIA_FEATURES, mediaFeaturesHandler);

  Serial.println("Setup Complete.");
}


void loop() {

  // Make sure we are connected to the MQTT broker.
  if (!gateway->isConnected()) {
    Serial.println("Reconnecting.");
    gateway->reconnect();
  }
}