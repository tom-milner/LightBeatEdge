//
// Created by Tom Milner on 31/01/2021.
//

// These are the config options that are required to set up an MQTT gateway.
// It differs from MQTT constants in that the MQTT Constants are predefined, whilst the MQTT Config options are determined by the library user.
struct WifiInfo {
  char * SSID;
  char * Password;
};

struct BrokerInfo {
  char * Address;
  unsigned int Port;
};

struct MQTTConfig {
  WifiInfo Wifi;
  BrokerInfo Broker;
};