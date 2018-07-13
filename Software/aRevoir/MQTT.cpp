//
// Created by elektron on 19/06/18.
//

#include "MQTT.h"

MQTT::MQTT(WiFiClient wifiClient, Display display, void(*callBack)(char* topic, byte* payload, unsigned int length)) : pubSubClient(wifiClient){
    this->display = display;
    this->callBack = callBack;
}

void MQTT::init(){
    pubSubClient.setServer(mqtt_server, port);
    //callBack("topic", reinterpret_cast<byte*>(5), 4);
    //pubSubClient.setCallback(callBack);

    //pubSubClient.connect(CLIENT_ID, USER,PASS);
    //reconnect();
}

void MQTT::reconnect() {
    Serial.println("Reconnecting...");
    while (!pubSubClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        if (pubSubClient.connect(CLIENT_ID, USER, PASS)) {
            Serial.println("connected");
            pubSubClient.subscribe(QUALIFICATION_CHAN);
            pubSubClient.subscribe(NOM_CHAN);
            pubSubClient.subscribe(PRENOM_CHAN);
        } else {
            Serial.print("failed, rc=");
            Serial.print(pubSubClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MQTT::manage() {
    if (!pubSubClient.connected()) {
        reconnect();
    }
    pubSubClient.loop();
}

void MQTT::setCallBack(void (*callBack)(char *, byte *, unsigned int)) {
    pubSubClient.setCallback(callBack);
}