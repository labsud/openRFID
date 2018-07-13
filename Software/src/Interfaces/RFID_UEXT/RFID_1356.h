//
// Created by elektron on 21/06/18.
//

#ifndef MQTT_ESP32_EVB_RFID_1356_H
#define MQTT_ESP32_EVB_RFID_1356_H

#include <HardwareSerial.h>
#include <Arduino.h>

class RFID_1356 {
public:
    RFID_1356();
    void init();
    void read(char* tag);

private:
    byte newTag[200];
    HardwareSerial rfidSerial;

    void dump_byte_array_hex(unsigned char *buffer, unsigned char bufferSize, char *text);

    byte c1;

    void utf8ascii(char *s);

    byte utf8ascii(byte ascii);
    void trim(char* string);
};


#endif //MQTT_ESP32_EVB_RFID_1356_H
