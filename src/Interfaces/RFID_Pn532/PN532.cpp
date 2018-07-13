//
// Created by elektron on 27/06/18.
//

#include "PN532.h"

PN532::PN532() : nfc(IRQ, RESET) {

}

bool PN532::init() {
    nfc.begin();
    delay(100);
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN53x board");
        return false;
    } else {
        // Got ok data, print it out!
        Serial.print("Found chip PN5");
        Serial.println((versiondata >> 24) & 0xFF, HEX);
        Serial.print("Firmware ver. ");
        Serial.print((versiondata >> 16) & 0xFF, DEC);
        Serial.print('.');
        Serial.println((versiondata >> 8) & 0xFF, DEC);

        // configure board to read RFID tags
        nfc.SAMConfig();
        return true;
    }
}

bool PN532::read(char *tag) {

    // Wait for an ISO14443A type cards (Mifare, etc.). When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    nfc.begin();
    nfc.SAMConfig();
    if(!nfc.getFirmwareVersion()) return false;
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        // Found a card!
        // turn the four byte UID of a mifare classic into a single variable #
        cardidentifier = uid[3];
        cardidentifier <<= 8;
        cardidentifier |= uid[2];
        cardidentifier <<= 8;
        cardidentifier |= uid[1];
        cardidentifier <<= 8;
        cardidentifier |= uid[0];

        sprintf(tag,"%lu", cardidentifier);
        Piezo::longBip(1);
        delay(1000);
    } else {
        memset(tag, NULL, sizeof(tag));
    }
    return true;
}
