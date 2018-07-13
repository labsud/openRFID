//
// Created by elektron on 19/06/18.
//

#include <MFRC522.h>
#include "RFID.h"

#define RST_PIN 12
#define SS_PIN 25

RFID::RFID() : SPI2(VSPI){

}

void RFID::init() {
    //SPI.begin();
    //SPI.begin(14,15,2);
    //SPI.begin(18, 5, 22);
    SPI.begin(18, 19, 23);
    //SPI2.begin();
    //SPI.begin(20, 19, 21);
    mfrc522.PCD_Init(SS_PIN, RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));

    Serial.print(F(": "));
    mfrc522.PCD_DumpVersionToSerial();
}

void RFID::manage(char* tag) {
    //char test[50];
    //memset(test, NULL, 50);//sizeof(test));
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print(F(": Card UID:"));
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size, tag);
        //Serial.println();
        //Serial.println(tag);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        /*char test1[3];
        char test2[3];
        char test3[3];
        for(int i=0; i<3; i++){
            test1[i] = test[i];
            test2[i] = test[i+3];
            test3[i] = test[i+6];
        }

        long tag = atoi(test1);
        Serial.println(tag);
        return tag;*/
    }
}

void RFID::dump_byte_array(byte *buffer, byte bufferSize, char *text) {
    char temp[4];
    memset(temp, NULL, sizeof(temp));

    for (byte i = 0; i < bufferSize; i++) {
        itoa(buffer[i],temp,10);
        strcat(text,temp);
        Serial.print(buffer[i], DEC);
        Serial.print(" ");
    }
    Serial.println("");
}
