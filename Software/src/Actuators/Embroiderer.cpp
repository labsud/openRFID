//
// Created by elektron on 12/07/18.
//

#include "Embroiderer.h"

Embroiderer::Embroiderer() {

}

void Embroiderer::init(int pinHall) {
    this->pinHall = pinHall;
    pinMode(pinHall, INPUT_PULLUP);
    pinMode(RELAY1, OUTPUT);
    digitalWrite(RELAY1, LOW);
}

bool Embroiderer::isUsed() {
    return isActive;
}

bool Embroiderer::isUsedByThisUser(char *tag) {
    Serial.print("Tag enregistré : ");
    Serial.println(userTag);
    return (strcmp(tag, userTag) == 0);
}

void Embroiderer::enable(char *userTag) {
    nbPoints = 0;
    isActive = true;
    strcpy(this->userTag, userTag);
    Serial.print("Brodeuse maintenant utilisée par ");
    Serial.println(userTag);
    digitalWrite(RELAY1, HIGH);

}

void Embroiderer::disable() {
    isActive = false;
    memset(userTag, NULL, sizeof(userTag));
    Serial.println("Brodeuse éteinte.");
    digitalWrite(RELAY1, LOW);
}

long Embroiderer::getNbPoints() {
    return nbPoints;
}

void Embroiderer::addPoint() {
    Serial.println("Point !");
    nbPoints++;
}
