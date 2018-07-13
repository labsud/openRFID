//
// Created by elektron on 20/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_LASER_H
#define MQTT_AUTH_REFACTOR_LASER_H

#include <esp32-hal.h>
#include "HardwareSerial.h"

class Laser {
public:
    Laser();
    void init(int pinSwitch);
    void enable(void(*function)(char *post, char *chan));
    void disable(void(*function)(char *post, char *chan));
    bool isUsed();
    bool isReady();
    bool getSwitchState();
    void prepare(char *userTag);
    void beginTimer();
    bool isUsedByThisUser(char* tag);
    unsigned long getUsingTime();

private:
    int pinSwitch;
    bool isActive;
    bool ready;
    long startingTimer;
    long timeToWait;
    char* laser_chan = "domotique/prise1/relay/0/set";
    char* enablePost = "1";
    char* disablePost = "0";
    char userTag[250];

};


#endif //MQTT_AUTH_REFACTOR_LASER_H
