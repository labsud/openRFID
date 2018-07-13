//
// Created by elektron on 12/07/18.
//

#ifndef MQTT_ESP32_EVB_EMBROIDERER_H
#define MQTT_ESP32_EVB_EMBROIDERER_H

#include "esp32-hal-gpio.h"
#include "HardwareSerial.h"

#define RELAY1 32

class Embroiderer {
public:
    Embroiderer();
    void init(int pinHall);
    bool isUsed();
    bool isUsedByThisUser(char* tag);
    void enable(char* userTag);
    void disable();
    long getNbPoints();
    void addPoint();

private:
    bool isActive;
    long nbPoints;
    int pinHall;
    char userTag[250];
};


#endif //MQTT_ESP32_EVB_EMBROIDERER_H
