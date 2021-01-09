

#include <Arduino.h>
#include <Gateway.h>
#include <Lights.h>
#include <ArduinoJson.h>

IGateway *gateway;
Lights lights;


void beatHandler(char *topic, byte *payload, unsigned int length) {
  StaticJsonDocument<40> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.println(error.c_str());
    error.code();
  }
  int duration = doc["duration"].as<int>();
//  int number = doc["number"].as<int>();

  lights.fadeByHueDelta(32, duration/2);
}


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
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
#endif

  Serial.begin(115200);

  delay(500);

  printBanner();

  // Setup the lights.
  lights.init(300);

  // Create gateway.
  gateway = createGateway(MQTT);
  gateway->init();

  // Setup message handlers.
  gateway->onReceive(GatewayConstants::Messages::BEAT, beatHandler);


  Serial.println("Setup Complete.");
}


void loop() {

  // Make sure we are connected to the MQTT broker.
  if (!gateway->isConnected()) {
    Serial.println("Reconnecting.");
    gateway->reconnect();
  }
}