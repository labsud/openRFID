//
// Created by elektron on 19/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_MQTT_H
#define MQTT_AUTH_REFACTOR_MQTT_H

#include <PubSubClient.h>
#include "WiFi.h"
#include "Interfaces/Display.h"

#define CLIENT_ID "arduinoClientBattoRFID"
#define USER "server"
#define PASS "csneya"

#define QUALIFICATION_CHAN "labsud/test/qualification"
#define NOM_CHAN "labsud/test/nom"
#define PRENOM_CHAN "labsud/test/prenom"

class MQTT {
private:
    PubSubClient pubSubClient;
    Display display;
    void(*callBack)(char* topic, byte* payload, unsigned int length);
    //const char* mqtt_server = "montpellier.labsud.org";
    //const int port = 1884;

public:
    MQTT(WiFiClient wifiClient, Display display, void(*callBack)(char* topic, byte* payload, unsigned int length));
    void init();
    void reconnect();
    void manage();
    void setCallBack(void(*callBack)(char* topic, byte* payload, unsigned int length));
};


#endif //MQTT_AUTH_REFACTOR_MQTT_H
