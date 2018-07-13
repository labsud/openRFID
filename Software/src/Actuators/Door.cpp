//
// Created by elektron on 27/06/18.
//

#include "Door.h"
#include <esp32-hal-gpio.h>
#include <HardwareSerial.h>

Door::Door() {
}

void Door::init(int pinReed) {
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);

    this->pinReed = pinReed;
    pinMode(pinReed, INPUT_PULLUP);

    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
}

void Door::turn(bool dir) {
    if(dir) {
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, LOW);
    } else {
        digitalWrite(RELAY1, LOW);
        digitalWrite(RELAY2, HIGH);
    }
}

void Door::stop(){
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
}

bool Door::isBlocked() {
    //check whether during 2ms current > trigger
    long lastTime = millis();
    while (millis() - lastTime <= 2) { //if during 2ms current > trigger
        if (analogRead(CURRENT_SENSOR) < TRIGGER_CURRENT_SENSOR) return false;
        delayMicroseconds(100);
    }
    return true;
}

void Door::lock() {
    if (isDoorClosed()) {
        for (int i = 0; i < NB_ATTEMPT_FOR_LOCKING; i++) {
            turn(LOCKING);
            delay(200); //To wait for the current peak
            long lastTime = millis();
            while (!isBlocked() && millis() - lastTime <= TURNING_DELAY_IF_NOT_BLOCKED) {
                turn(LOCKING);
            }
            stop();
            delay(100);
        }
    }
    isLocked = true;
}

void Door::unLock() {
    for (int i=0; i < NB_ATTEMPT_FOR_LOCKING; i++) {
        turn(UNLOCKING);
        delay(200); //To wait for the current peak
        long lastTime = millis();
        while (!isBlocked() && millis() - lastTime <= TURNING_DELAY_IF_NOT_BLOCKED) {
            turn(UNLOCKING);
        }
        stop();
        delay(100);
    }
    isLocked = false;
}

bool Door::isDoorClosed() {
    return !digitalRead(pinReed);
}

bool Door::getState() {
    return isLocked;
}
