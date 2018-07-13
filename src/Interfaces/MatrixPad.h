//
// Created by elektron on 20/06/18.
//

#ifndef KEYPAD_MATRIXPAD_H
#define KEYPAD_MATRIXPAD_H

#include "Keypad.h"
#include "Interfaces/Piezo.h"

/*PINOUT
 * KeyPad   |   GPIO (ESP32-EVB)
 * 1        |   34
 * 2        |   35
 * 3        |   36
 * 4        |   39
 * 5        |   2
 * 6        |   3
 * 7        |   15
 * 8        |   17
*/


class MatrixPad {
public:
    MatrixPad();
    char getKey();
    void init();
    void getWord(char* wordToChange);
    void getWordInProgress(char* wordInProgressToChange);
    void beginTimer(long timeToWait);
    bool checkTimer();
    void stopTimer();

private:

    //byte colPins[4] = {18,19,21,22}; //Lignes
    byte colPins[4] = {2,3,4,14};
    byte rowPins[4] = {15,17,18,21}; //Colonnes

    char keys[4][4] = {
            {'1','2','3','A'},
            {'4','5','6','B'},
            {'7','8','9','C'},
            {'*','0','#','D'}};
    Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

    char wordInProgress[12];
    char lastFinishedWord[12];
    bool isWordFinished;
    void resetWord();
    bool isTimerEnabled;
    long timeToWait;
    long startingTime;
};


#endif //KEYPAD_MATRIXPAD_H
