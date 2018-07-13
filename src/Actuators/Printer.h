//
// Created by elektron on 12/07/18.
//

#ifndef MQTT_ESP32_EVB_PRINTER_H
#define MQTT_ESP32_EVB_PRINTER_H

#include "esp32-hal-gpio.h"
#include "HardwareSerial.h"

#define RELAY1 32
#define RELAY2 33

class Printer {
public:
    Printer();
    void init(int pinSwitch);
    bool isReady();
    bool isUsed();
    bool getSwitchState();
    bool isUsedByThisUser(char *tag);
    void prepare(char *userTag);
    void enable();
    void disable();
    void beginTimer();
    unsigned long getUsingTime();

private:
    int pinSwitch;
    bool ready;
    bool isActive;
    long startingTimer;
    char userTag[250];
};


#endif //MQTT_ESP32_EVB_PRINTER_H
