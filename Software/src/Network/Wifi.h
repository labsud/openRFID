//
// Created by elektron on 19/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_WIFI_H
#define MQTT_AUTH_REFACTOR_WIFI_H

#include "WiFi.h"

class Wifi {
private:
    WiFiClient espWifiClient;

public:
    Wifi();
    void init(char *ssid, char *password);
    void init(char *ssid, char *password, IPAddress ip, IPAddress subnet, IPAddress gateway, IPAddress dns);
    String getIP();
    WiFiClient & getClient();
};


#endif //MQTT_AUTH_REFACTOR_WIFI_H
