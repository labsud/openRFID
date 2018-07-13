//
// Created by elektron on 21/06/18.
//

#include "RFID_1356.h"
#include "Arduino.h"
#include "stdio.h"

RFID_1356::RFID_1356() : rfidSerial(1){
}

void RFID_1356::init() {
    rfidSerial.begin(38400, SERIAL_8E1, 36, 3);
    //rfidSerial.begin(38400);
    delay(50);
    rfidSerial.println("er0");
    Serial.println("RFID init complete");
}

void RFID_1356::read(char* tag) {
    int temp = rfidSerial.read();

    if (temp == -1){
        //Serial.println("No new tag");
        memset(tag, NULL, sizeof(tag));
    } else {
        rfidSerial.readBytes(newTag, sizeof(newTag));
        dump_byte_array_hex(newTag, sizeof(newTag), tag);
        trim(tag);
        //utf8ascii(tag);
        //Serial.println(tag);

        //String chaine = rfidSerial.readString();
        //strcpy(tag, chaine.c_str());
        memset(newTag, NULL, sizeof(newTag));
    }
}

void RFID_1356::trim(char *string) {
    int index = strlen(string) - 1;
    Serial.println(index);
    Serial.println(string[index]);
    while (string[index] == '0'){
        string[index] = '\0';
        index--;
    }
}

void RFID_1356::utf8ascii(char* s)
{
    int k=0;
    char c;
    for (int i=0; i<strlen(s); i++)
    {
        c = utf8ascii(s[i]);
        if (c!=0)
            s[k++]=c;
    }
    s[k]=0;
}

byte RFID_1356::utf8ascii(byte ascii) {
    if ( ascii<128 )   // Standard ASCII-set 0..0x7F handling
    {   c1=0;
        return( ascii );
    }

    // get previous input
    byte last = c1;   // get last char
    c1=ascii;         // remember actual character

    switch (last)     // conversion depending on first UTF8-character
    {   case 0xC2: return  (ascii);  break;
        case 0xC3: return  (ascii | 0xC0);  break;
        case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
    }

    return  (0);                                     // otherwise: return zero, if character has to be ignored
}

void RFID_1356::dump_byte_array_hex(byte *buffer, byte bufferSize, char *text) {
    char temp[4];
    memset(temp, NULL, sizeof(temp));

    for (byte i = 0; i < bufferSize; i++) {
        itoa(buffer[i],temp,10);
        strcat(text,temp);
        /*Serial.print(buffer[i], DEC);
        Serial.print(" ");*/
    }
    //Serial.println("");
}