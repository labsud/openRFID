//
// Created by elektron on 20/06/18.
//

#include "Laser.h"

Laser::Laser(){
}

void Laser::init(int pinSwitch) {
    this->pinSwitch = pinSwitch;
    pinMode(pinSwitch, INPUT_PULLUP);
}

void Laser::prepare(char *userTag) {
    strcpy(this->userTag, userTag);
    Serial.print("Laser now used by ");
    Serial.println(userTag);
    ready = true;
}

void Laser::enable(void(*function)(char *post, char *chan)) {
    function(enablePost, laser_chan);
    isActive = true;
    Serial.println("Laser en fonctionnement");
    beginTimer();
}

void Laser::disable(void(*function)(char *post, char *chan)) {
    function(disablePost, laser_chan);
    isActive = false;
    ready = false;
    memset(userTag, NULL, sizeof(userTag));
    Serial.println("Laser éteint.");
}

bool Laser::isUsed() {
    return isActive;
}

void Laser::beginTimer() {
    startingTimer = millis();
}

bool Laser::isUsedByThisUser(char* tag) {
    Serial.print("Tag enregistré : ");
    Serial.println(userTag);
    return (strcmp(tag, userTag) == 0);
}

unsigned long Laser::getUsingTime() {
    return (millis() - startingTimer)/(1000);
}

bool Laser::isReady() {
    return ready;
}

bool Laser::getSwitchState() {
    return !digitalRead(pinSwitch);
}
