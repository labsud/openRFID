//
// Created by elektron on 27/06/18.
//

#ifndef MQTT_ESP32_EVB_PIEZO_H
#define MQTT_ESP32_EVB_PIEZO_H

#include "Arduino.h"

#define PIN_PIEZO 19

class Piezo {
public:
    static void init();
    static void bip(int nbBips);
    static void  longBip(int nbBips);
};


#endif //MQTT_ESP32_EVB_PIEZO_H
