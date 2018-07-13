//
// Created by elektron on 27/06/18.
//

#include "Piezo.h"

void Piezo::init() {
    pinMode(PIN_PIEZO, OUTPUT);
    digitalWrite(PIN_PIEZO, LOW);
}

void Piezo::bip(int nbBips) {
    for(int i=0; i<nbBips; i++) {
        digitalWrite(PIN_PIEZO, HIGH);
        delay(100);
        digitalWrite(PIN_PIEZO, LOW);
        delay(100);
    }
}

void Piezo::longBip(int nbBips) {
    for(int i=0; i<nbBips; i++) {
        digitalWrite(PIN_PIEZO, HIGH);
        delay(300);
        digitalWrite(PIN_PIEZO, LOW);
        delay(300);
    }
}
