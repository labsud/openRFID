//
// Created by elektron on 19/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_DISPLAY_H
#define MQTT_AUTH_REFACTOR_DISPLAY_H

#include "SSD1306.h"

class Display {
public:
    Display();
    void init();
    void displayText(char* text);
    void displayText(char* text, int y);
    void displayText(char* text, int x, int y);
    void eraseTextZone();
    void diplayCenterText(char* text);
    void displayLogo();
    void setBackground(char *ip);
    void beginTimer(long timeToWait);
    void stopTimer();
    bool checkTimer();

private:
    SSD1306 ssd1306;
    long startingTime;
    long timeToWait;
    bool isTimerEnabled;
};


#endif //MQTT_AUTH_REFACTOR_DISPLAY_H
