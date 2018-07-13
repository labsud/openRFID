//
// Created by elektron on 19/06/18.
//

#include "Interfaces/Display.h"
#include "Interfaces/MatrixPad.h"
#include "Interfaces/RFID_Pn532/PN532.h"

#include "Network/Wifi.h"
#include "Network/Ethernet.h"
#include "PubSubClient.h"

#include "SD/Utils/SDWrapper.h"
#include "SD/Config/Config.h"

#include "Actuators/Door.h"
#include "Actuators/Laser.h"
#include "Actuators/Printer.h"
#include "Actuators/Embroiderer.h"

/*To use MOD-1356 by Olimex in UEXT*/
//#include "Interfaces/RFID_UEXT/RFID_1356.h"
//RFID_1356 rfid1356;

/*To use MFRC522 with SPI (maybe not compatible with everything)*/
//#include "Interfaces/RFID_Mfrc522/RFID.h"
//RFID rfid;

Wifi espWifi;
PubSubClient pubSubClient(espWifi.getClient());

ESP32Ethernet espEthernet;
/*EthernetClient client;
PubSubClient pubSubClientEthernet(client);*/

Config config;

PN532 rfid_pn532;
MatrixPad matrix;

Display display;

Laser laser;
Door door;
Printer printer;
Embroiderer embroiderer;

/*Fonctions et variables se trouvant dans Network/MQTT.h*/
/**/extern void initMQTT();
/**/extern void callBack(char *topic, byte *payload, unsigned int length);
/**/extern void reconnect();
/**/extern void manageMQTT();
/**/extern void publishMQTT(char * post, char* chan);
/**/extern void manageMessageMQTT();
/*-------------------------------------------------------*/

void WiFiEvent(WiFiEvent_t event);

void verifyTagForLaser();
void useMasterPasswordForLaser();
void verifyPasswordForLaser();

void verifyTagForPrinter();
void useMasterPasswordForPrinter();
void verifyPasswordForPrinter();

void verifyTagForDoor();
void useMasterPasswordForDoor();
void verifyPasswordForDoor();

void verifyTagForEmbroiderer();
void useMasterPasswordForEmbroiderer();
void verifyPasswordForEmbroiderer();
void addPointISR();

char tag[200];
char lastTag[200];
char passWord[12];
char passWordInProgress[12];
char incomingPayload[50];
char incomingTopic[50];

IPAddress ip;
IPAddress subnet;
IPAddress gateway;
IPAddress dns;

void setup(){
    Serial.begin(115200);
    delay(2000);
    Piezo::init();

    config.init("/config.ini");
    config.getConfig();
    Piezo::bip(1);
    delay(500);

    if (config.usingDisplay()) {
        display.init();
        delay(2000);
    }

    if(config.usingStaticIP()) {
        char* temp1;
        char* temp2;
        char* temp3;
        char* temp4;

        temp1 = strtok(config.getIp(), ".");
        for (int i=0; temp1!=NULL; i++) {
            ip[i] = atoi(temp1);
            temp1 = strtok(NULL, ".");
        }
        temp2 = strtok(config.getSubnet(), ".");
        for (int i=0; temp2!=NULL; i++) {
            subnet[i] = atoi(temp2);
            temp2 = strtok(NULL, ".");
        }
        temp3 = strtok(config.getGateway(), ".");
        for (int i=0; temp3!=NULL; i++) {
            gateway[i] = atoi(temp3);
            temp3 = strtok(NULL, ".");
        }
        temp4 = strtok(config.getDns(), ".");
        for (int i=0; temp4!=NULL; i++) {
            dns[i] = atoi(temp4);
            temp4 = strtok(NULL, ".");
        }
    }

    if (config.usingWifi()) {
        if (config.usingDisplay()) display.displayText("Initializing WiFi...");
        if (config.usingStaticIP()) {
            espWifi.init(config.getSsid(), config.getWifiPassword(), ip, subnet, gateway, dns);
        } else {
            espWifi.init(config.getSsid(), config.getWifiPassword());
        }
    } else {
        if (config.usingDisplay()) display.displayText("Initializing Ethernet...");
        delay(100);
        WiFi.onEvent(WiFiEvent);
        delay(100);
        if (config.usingStaticIP()) {
            espEthernet.begin(ip, subnet,gateway);
        } else {
            espEthernet.begin();
        }
        delay(10000);
    }

    if (config.usingDisplay()) display.displayText("Initializing MQTT...");
    initMQTT();
    delay(500);

    if (config.usingDisplay()) display.displayText("Initializing RFID...");
    //rfid.init();
    //rfid1356.init();
    while(!rfid_pn532.init()) {delay(500);}
    delay(500);

    if (config.usingKeypad()) {
        if (config.usingDisplay()) display.displayText("Initializing Keypad...");
        matrix.init();
        delay(500);
    }

    if (config.getActuator() == LASER) {
        if (config.usingDisplay()) display.displayText("Initializing Laser...");
        laser.init(config.getSwitchPin());
        delay(500);
    } else if (config.getActuator() == DOOR) {
        if (config.usingDisplay()) display.displayText("Initializing Lock...");
        door.init(config.getIlsPin());
        delay(500);
    } else if (config.getActuator() == PRINTER) {
        if (config.usingDisplay()) display.displayText("Initializing Printer...");
        printer.init(config.getSwitchPin());
        delay(500);
    } else if (config.getActuator() == EMBROIDERER) {
        if (config.usingDisplay()) display.displayText("Initializing Embroiderer...");
        printer.init(config.getHallPin());
        attachInterrupt(config.getHallPin(), addPointISR, FALLING);
        delay(500);
    }

    if (config.usingDisplay()) display.setBackground((char *) espWifi.getIP().c_str());
    Piezo::bip(2);
}

void loop(){
    /*Reception et traitement des messages MQTT*/
    manageMQTT();

    if(incomingPayload[0] != '\0') {
        manageMessageMQTT();
        if (config.usingDisplay()) display.beginTimer(5000);
    }
    /*-----------------------------------------*/

    /*Lecture du badge RFID et envoie sur le serveur*/
    //rfid1356.read(tag);
    if(!rfid_pn532.read(tag)) {
        Serial.println("Houston on a un problème");
        Wire.endTransmission();
        rfid_pn532.init();
        Serial.println("Redémarrage de l'I2C");
        delay(300);
    }

    if(tag[0] != '\0'){ //si le tableau n'est pas vide
        Serial.print("Carte badgée : ");
        Serial.println(tag);
        strcpy(lastTag, tag);
        if (strcmp(tag, config.getMasterTag()) == 0) {
            Serial.println("Big Boss");
            Piezo::bip(3);
            if (config.getActuator() == DOOR) useMasterPasswordForDoor();
            else if (config.getActuator() == LASER) useMasterPasswordForLaser();
            else if (config.getActuator() == PRINTER) useMasterPasswordForPrinter();
            else if (config.getActuator() == EMBROIDERER) useMasterPasswordForEmbroiderer();
        } else {
            Serial.println("Pas big boss");
            if (config.getActuator() == DOOR) verifyTagForDoor();
            else if (config.getActuator() == LASER) verifyTagForLaser();
            else if (config.getActuator() == PRINTER) verifyTagForPrinter();
            else if (config.getActuator() == EMBROIDERER) verifyTagForEmbroiderer();
        }
    }
    /*-----------------------------------------------*/

    if (config.usingKeypad()) {
        /*Récupération ce qui est en train d'être tapé sur le clavier et affichage*/
        matrix.getWordInProgress(passWordInProgress);

        if (passWordInProgress[0] != '\0') {
            if (config.usingDisplay()) display.displayText(passWordInProgress);
        }
        /*------------------------------------------------------------------------*/

        /*Récupération du mot de passe terminé et déblocage si autorisé (+backdoor)*/
        matrix.getWord(passWord);

        if (passWord[0] != '\0') {
            strcpy(lastTag, passWord);

            if (strcmp(passWord, config.getMasterPassword()) == 0) {
                strcpy(tag, config.getMasterPassword());
                strcpy(lastTag, tag);

                if (config.getActuator() == DOOR) useMasterPasswordForDoor();
                else if (config.getActuator() == LASER) useMasterPasswordForLaser();
                else if (config.getActuator() == PRINTER) useMasterPasswordForPrinter();
                else if (config.getActuator() == EMBROIDERER) useMasterPasswordForEmbroiderer();

            } else {
                Serial.println(passWord);

                if (config.getActuator() == DOOR) verifyPasswordForDoor();
                else if (config.getActuator() == LASER) verifyPasswordForLaser();
                else if (config.getActuator() == PRINTER) verifyPasswordForPrinter();
                else if (config.getActuator() == EMBROIDERER) verifyPasswordForEmbroiderer();
            }
            if (config.usingDisplay()) display.beginTimer(5000);
        }
        /*-----------------------------------------------------------------------*/
    }

    /*Vérification du contacteur pour déblocage total*/
    if (config.getActuator() == PRINTER) {
        if (printer.getSwitchState() && printer.isReady() && !printer.isUsed()) {
            printer.enable();
            delay(100);
            Serial.println("Imrimante débloquée");
        } else if (!printer.getSwitchState() && printer.isUsed()) {
            printer.disable();
            Serial.println("Imprimante bloquée.");
            Serial.print("Imprimante utilisée pendant ");
            Serial.print(printer.getUsingTime());
            Serial.println(" secondes.");
        }
    } else if (config.getActuator() == LASER) {
        if (laser.getSwitchState() && laser.isReady() && !laser.isUsed()) {
            void (*pFunction)(char *post, char *chan);
            pFunction = publishMQTT;
            laser.enable(pFunction);
        } else if (!laser.getSwitchState() && laser.isUsed()) {
            void (*pFunction)(char *post, char *chan);
            pFunction = publishMQTT;
            laser.disable(pFunction);
            Serial.print("Laser utilisé pendant ");
            Serial.print(laser.getUsingTime());
            Serial.println(" secondes.");
        }
    }
    /*------------------------------------------------*/

    /*Tout ce qui est relatif au temps, timeout du clavier, effacement de l'écran, désactivation de la machine si utilisation sur timer*/
    if(matrix.checkTimer()){
        matrix.stopTimer();
        Serial.println("Effacement du mot en cours");
        if (config.usingDisplay()) display.setBackground((char *) espWifi.getIP().c_str());
    }

    if (config.usingDisplay()) {
        if(display.checkTimer()){
            display.stopTimer();
            Serial.println("Effacement de l'écran");
            display.setBackground((char *) espWifi.getIP().c_str());
        }
    }

    /*if(laser.isTimerFinished()) {
        void (*pFunction)(char * post, char* chan);
        pFunction = publishMQTT;
        laser.disable(pFunction);
    }*/
    /*----------------------------------------------------------------------------------------------------------------*/

    /*Effacement des tableaux*/
    memset(incomingPayload, NULL, sizeof(incomingPayload));
    memset(incomingTopic, NULL, sizeof(incomingTopic));
    memset(tag, NULL, sizeof(tag)); //on vide le tableau
    //delay(50);
    /*----------------------*/
}

void verifyTagForLaser() {
    if (laser.isUsed()){
        Piezo::longBip(2);
        if (config.usingDisplay()) display.displayText("Already in use");
        if (config.usingDisplay()) display.beginTimer(5000);
    } else {
        publishMQTT(lastTag, config.getTagChan());
    }
}

void useMasterPasswordForLaser() {
    if(!laser.isUsed()) {
        if (config.usingDisplay()) display.displayText("Big Boss");
        laser.prepare(lastTag);
    } else {
        if (config.usingDisplay()) display.displayText("Already in use");
    }
}

void verifyPasswordForLaser() {
    if (laser.isUsed()){
        Piezo::longBip(2);
        if (config.usingDisplay()) display.displayText("Already in use");
        if (config.usingDisplay()) display.beginTimer(5000);
    } else {
        publishMQTT(passWord, config.getPasswordChan());
    }
}

void verifyTagForDoor() {
        publishMQTT(lastTag, config.getTagChan());
}

void useMasterPasswordForDoor() {
    if (config.usingDisplay()) display.displayText("Big Boss");
    if(door.getState() == LOCKED) {
        door.unLock();
    } else {
        door.lock();
    }
}

void verifyPasswordForDoor() {
    publishMQTT(passWord, config.getPasswordChan());
}

void verifyTagForPrinter() {
    if (printer.isUsed()) {
        if (!printer.isUsedByThisUser(lastTag)) {
            Piezo::longBip(2);
            if (config.usingDisplay()) display.displayText("Already in use");
            if (config.usingDisplay()) display.beginTimer(5000);
        }
    } else {
        publishMQTT(lastTag, config.getTagChan());
    }
}

void useMasterPasswordForPrinter() {
    if(!printer.isUsed()) {
        if (config.usingDisplay()) display.displayText("Big Boss");
        printer.prepare(lastTag);
    } else {
        if (config.usingDisplay()) display.displayText("Already in use");
    }
}

void verifyPasswordForPrinter() {
    if (printer.isUsed() && !printer.isUsedByThisUser(lastTag)){
        Piezo::longBip(2);
        if (config.usingDisplay()) display.displayText("Already in use");
        if (config.usingDisplay()) display.beginTimer(5000);
    } else {
        publishMQTT(passWord, config.getPasswordChan());
    }
}

void verifyTagForEmbroiderer() {
    if (embroiderer.isUsed() && !embroiderer.isUsedByThisUser(lastTag)) {
        if (!embroiderer.isUsedByThisUser(lastTag)) {
            Piezo::longBip(2);
            if (config.usingDisplay()) display.displayText("Already in use");
            if (config.usingDisplay()) display.beginTimer(5000);
        }
    } else {
        publishMQTT(lastTag, config.getTagChan());
    }
}

void useMasterPasswordForEmbroiderer() {
    if(!embroiderer.isUsed()) {
        if (config.usingDisplay()) display.displayText("Big Boss");
        embroiderer.enable(lastTag);
    } else if (embroiderer.isUsedByThisUser(lastTag)){
        embroiderer.disable();
        Serial.print("Brodeuse utilisée avec ");
        Serial.print(embroiderer.getNbPoints());
        Serial.println(" points");
    }
    else {
        if (config.usingDisplay()) display.displayText("Already in use");
    }
}

void verifyPasswordForEmbroiderer() {
    if (embroiderer.isUsed() && !embroiderer.isUsedByThisUser(lastTag)){
        Piezo::longBip(2);
        if (config.usingDisplay()) display.displayText("Already in use");
        if (config.usingDisplay()) display.beginTimer(5000);
    } else {
        publishMQTT(passWord, config.getPasswordChan());
    }
}

void addPointISR() {
    embroiderer.addPoint();
}

void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_ETH_START:
            //set eth hostname here
            espEthernet.setHostname("esp32-eth");
            Serial.println("ETH started");
            Serial.print("ETH MAC: ");
            Serial.println(espEthernet.getMacAddress());
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("ETH connected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.print("ETH IPv4: ");
            Serial.println(espEthernet.localIP());
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("ETH disconnected");
            break;
        default:
            break;
    }
}

void inter() {
    Serial.println("INTERRUPTION !");
}