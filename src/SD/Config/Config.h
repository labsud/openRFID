//
// Created by elektron on 25/06/18.
//

#ifndef MQTT_ESP32_EVB_CONFIG_H
#define MQTT_ESP32_EVB_CONFIG_H

#include "INIReader.h"
#include <HardwareSerial.h>
#include "../Utils/SDWrapper.h"

#define FILESIZE 5000
#define CHAN_SIZE 30

#define DOOR 1
#define LASER 2
#define PRINTER 3
#define EMBROIDERER 4

class Config {
public:
    Config();
    void getConfigString();
    void getConfig();
    void init(char* configFile);

    char* getIp();
    char* getSubnet();
    char* getGateway();
    char* getDns();

    char* getSsid();
    char* getWifiPassword();

    char* getClientId();
    char* getServer();
    int getPort();

    char* getTagChan();
    char* getPasswordChan();
    char* getQualificationChan();
    char* getPointsChan();
    char* getNameChan();
    char* getSurnameChan();

    char* getMasterPassword();
    char* getMasterTag();

    int getActuator();

    bool usingWifi();
    bool usingStaticIP();
    bool usingDisplay();
    bool usingKeypad();

    int getIlsPin();
    int getHallPin();
    int getSwitchPin();

private:
    char configString[FILESIZE];

    char masterPassword[16];
    char masterTag[16];

    char ssid[30];
    char wifiPassword[40];

    char ip[20];
    char subnet[20];
    char gateway[20];
    char dns[20];

    char mqttClientId[30];
    char mqttServer[40];
    int mqttPort;

    char tagChan[CHAN_SIZE];
    char passwordChan[CHAN_SIZE];
    char qualificationChan[CHAN_SIZE];
    char pointsChan[CHAN_SIZE];
    char nameChan[CHAN_SIZE];
    char surnameChan[CHAN_SIZE];

    char orderChan[CHAN_SIZE];

    char actuator[20];

    bool isUsedWifi;
    bool isStaticIP;
    bool isUsedDisplay;
    bool isUsedKeypad;

    int ilsPin;
    int hallPin;
    int switchPin;
};


#endif //MQTT_ESP32_EVB_CONFIG_H
