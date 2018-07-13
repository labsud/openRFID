//
// Created by elektron on 20/06/18.
//

#include "MatrixPad.h"

MatrixPad::MatrixPad() {
    Serial.println("Clavier pret");
}

char MatrixPad::getKey() {
    char key = keypad.getKey();
    if(key != NO_KEY && key != '*' && key != 'B' && key != 'C' && key != 'D') {
        if (key == '#') {
            resetWord();
            Serial.println("Word reseted");
            Piezo::longBip(1);

        } else if (key == 'A') {
            isWordFinished = true;
            strcpy(lastFinishedWord, wordInProgress);
            resetWord();
            stopTimer();
            Piezo::bip(2);
            //Serial.print("Word validated : ");
            //Serial.println(lastFinishedWord);

        } else if (strlen(wordInProgress) < 11) {
            isWordFinished = false;
            wordInProgress[strlen(wordInProgress)] = key;
            Piezo::bip(1);
            //Serial.print("Word in progress : ");
            //Serial.println(wordInProgress);

        } else {
            isWordFinished = false;
            resetWord();
            stopTimer();
            Serial.println("Error, too long passWord");
            Piezo::longBip(1);
        }

        beginTimer(5000);
    }
    return key;
}

void MatrixPad::init() {
    unsigned int time_hold = 4;
    keypad.setHoldTime(time_hold);
    unsigned int time_anti_rebond = 4;  //4 ms
    keypad.setDebounceTime(time_anti_rebond);
}

void MatrixPad::resetWord() {
    memset(wordInProgress, NULL, sizeof(wordInProgress));
}

void MatrixPad::getWord(char *wordToChange) {
    this->getKey();
    if ( isWordFinished) {
        strcpy(wordToChange, lastFinishedWord);
        Serial.print("Le mot complet est : ");
        Serial.println(lastFinishedWord);
        isWordFinished = false;
        delay(50);
    } else {
        memset(wordToChange, '\0', sizeof(wordToChange));
    }
}

void MatrixPad::getWordInProgress(char *wordInProgressToChange) {
    strcpy(wordInProgressToChange, wordInProgress);
}

void MatrixPad::beginTimer(long timeToWait) {
    isTimerEnabled = true;
    startingTime = millis();
    this->timeToWait = timeToWait;
}

bool MatrixPad::checkTimer() {
    if(isTimerEnabled) {
        return millis() - startingTime >= timeToWait;
    } else {
        return false;
    }
}

void MatrixPad::stopTimer() {
    isTimerEnabled = false;
    resetWord();
}