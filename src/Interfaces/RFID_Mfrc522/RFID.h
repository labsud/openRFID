//
// Created by elektron on 19/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_RFID_H
#define MQTT_AUTH_REFACTOR_RFID_H

#include <SPI.h>
#include <MFRC522.h>

/*PINOUT
 * MFRC522  |   GPIO (ESP32-EVB)
 * SDA (=SS)|   4
 * SCK      |   14 (HS2-CLK)
 * MOSI     |   2 (HS2_DATA0)
 * MISO     |   15 (HS2_CMD)
 * IRQ      |   not used
 * GND      |   GND
 * RST      |   12
 * 3,3V     |   3,3V
*/

class RFID {
private:
    MFRC522 mfrc522;
    SPIClass SPI2;
public:
    RFID();
    void init();
    void manage(char* tag);
    void dump_byte_array(byte *buffer, byte bufferSize, char* text);
};


#endif //MQTT_AUTH_REFACTOR_RFID_H
