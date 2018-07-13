//
// Created by elektron on 12/07/18.
//

#include "Printer.h"

Printer::Printer() {

}

void Printer::init(int pinSwitch) {
    this->pinSwitch = pinSwitch;
    pinMode(pinSwitch, INPUT_PULLUP);
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);

    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
}

void Printer::prepare(char* userTag) {
    strcpy(this->userTag, userTag);
    Serial.print("Printer now used by ");
    Serial.println(userTag);
    digitalWrite(RELAY1, HIGH);
    ready = true;
}

void Printer::enable() {
    beginTimer();
    digitalWrite(RELAY2, HIGH);
    isActive = true;
    Serial.println("Imprimante en fonctionnement");
}

void Printer::disable() {
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    isActive = false;
    ready = false;
    memset(userTag, NULL, sizeof(userTag));
}

void Printer::beginTimer() {
    startingTimer = millis();
}

unsigned long Printer::getUsingTime() {
    return (millis() - startingTimer)/(1000);
}

bool Printer::isUsed() {
    return isActive;
}

bool Printer::isReady() {
    return ready;
}

bool Printer::isUsedByThisUser(char* tag) {
    Serial.print("Tag enregistré : ");
    Serial.println(userTag);
    return (strcmp(tag, userTag) == 0);
}

bool Printer::getSwitchState() {
    return !digitalRead(pinSwitch);
}
