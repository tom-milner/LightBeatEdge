#include <Arduino.h>
#include <Gateway.h>


// This function is called whenever we receive a message via mqtt.
void beatHandler(char *topic, byte *payload, unsigned int length) {
   Serial.print(topic);
   Serial.print(": ");
   for (int i = 0; i < length; i++) {
   Serial.print((char) payload[i]);
   }
   Serial.println();
}

Gateway *gateway;

void setup() {
  gateway = new Gateway();


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

  gateway->init(MQTT);

  // Setup message handlers.
  gateway->onReceive(Constants::BEAT, beatHandler);

  Serial.println("Setup Complete.");
}



void loop() {

  // Make sure we are connected to the MQTT broker.
  if (!gateway->isConnected()) {
    Serial.println("Reconnecting.");
    gateway->reconnect();
  }

}