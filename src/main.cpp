

#include <Arduino.h>
#include <Gateway.h>
#include <Lights.h>


Gateway * gateway;
Lights lights;


void printMQTT(char * topic, byte* payload, unsigned int length){
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

void beatHandler(char *topic, byte *payload, unsigned int length) {
  printMQTT(topic, payload, length);
  lights.setAllHue(random(255));
}


void setup() {


//  Turn off BLE on esp32 (apparently this decreases WiFi latency).
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

  gateway = createGateway(MQTT);
  gateway->init();
  lights.init(300);

  // Setup message handlers.
  gateway->onReceive(BaseConstants::Messages::BEAT, beatHandler);

  Serial.println("Setup Complete.");
}


void loop() {

  // Make sure we are connected to the MQTT broker.
  if (!gateway->isConnected()) {
    Serial.println("Reconnecting.");
    gateway->reconnect();
  }
}