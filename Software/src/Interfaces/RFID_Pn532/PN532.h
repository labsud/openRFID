//
// Created by elektron on 27/06/18.
//

#ifndef MQTT_ESP32_EVB_PN532_H
#define MQTT_ESP32_EVB_PN532_H

#include "Adafruit_PN532.h"
#include "Interfaces/Piezo.h"

#define IRQ 0 // this trace must be cut and rewired to work on Leonardo
#define RESET 0

class PN532 {
public:
    PN532();
    bool init();
    bool read(char* tag);
private:
    Adafruit_PN532 nfc;
    uint8_t uid[8]; // Buffer to store the returned UID
    uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint32_t cardidentifier = 0;
};


#endif //MQTT_ESP32_EVB_PN532_H
