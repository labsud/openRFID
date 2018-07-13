//
// Created by elektron on 19/06/18.
//

#ifndef MQTT_AUTH_REFACTOR_MQTT_CPP
#define MQTT_AUTH_REFACTOR_MQTT_CPP

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <Interfaces/Piezo.h>
#include <Actuators/Door.h>
#include <SD/Config/Config.h>
#include "Interfaces/Display.h"
#include "Network/Wifi.h"
#include "Actuators/Laser.h"
#include "Actuators/Printer.h"
#include "Actuators/Embroiderer.h"

void initMQTT();
void callBack(char *topic, byte *payload, unsigned int length);
void reconnect();
void manageMQTT();
void manageMessageMQTT();
void verifyAndManageLaser();
void verifyAndManagePrinter();
void verifyAndManageDoor();
void verifyAndManageEmbroiderer();
void log();

extern PubSubClient pubSubClient;
extern Display display;
extern Wifi espWifi;
extern char incomingPayload[50];
extern char incomingTopic[50];
extern char tag[200];
extern char lastTag[200];
extern Config config;

extern Laser laser;
extern Door door;
extern Printer printer;
extern Embroiderer embroiderer;

#define LOG_CHAN "labsud/door/log"


void initMQTT()
{
    pubSubClient.setServer(config.getServer(), config.getPort());
    pubSubClient.setCallback(callBack);
    reconnect();
}

/*Est appellée automatiquement par la lib MQTT
 * Permet de récupérer le topic et le payload
 * stocke le topic dans le tableau externe incomingPayload*/
void callBack(char *topic, byte *payload, unsigned int length) {

    for (int i = 0; i < length; i++) {
        incomingPayload[i] = payload[i];
    }

    strcpy(incomingTopic, topic);
}

void reconnect() {
    while (!pubSubClient.connected()) {
        Serial.print("Attempting MQTT connection...");

        if (pubSubClient.connect(config.getClientId())) {
            Serial.println("connected");
            pubSubClient.subscribe(config.getQualificationChan());
            pubSubClient.subscribe(config.getNameChan());
            pubSubClient.subscribe(config.getSurnameChan());
        } else {
            Serial.print("failed, rc=");
            Serial.print(pubSubClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void manageMQTT()
{
    if (!pubSubClient.connected()) {
        reconnect();
    }
    pubSubClient.loop();
}

/*Envoie post sur chan*/
void publishMQTT(char * post, char* chan) {
    pubSubClient.publish(chan, post);
    Serial.print("Published on [");
    Serial.print(chan);
    Serial.print("] : ");
    Serial.println(post);
}

/*Permet de traiter tous les messages arrivant de MQTT
 * Affiche le nom et prénom de la personne ayant badgé
 * Si autorisé, allume, éteint ou avertit de l'utilisation de la machine*/
void manageMessageMQTT() {
    Serial.print("Incoming MQTT message : [");
    Serial.print(incomingTopic);
    Serial.print("] ");
    Serial.println(incomingPayload);

    if(strcmp(incomingTopic, config.getSurnameChan()) == 0) {
        Serial.print("Prénom : ");
        Serial.println(incomingPayload);
        if (config.usingDisplay()) display.displayText(incomingPayload, 64, 22);
    }

    if(strcmp(incomingTopic, config.getNameChan()) == 0) {
        Serial.print("Nom : ");
        Serial.println(incomingPayload);
        if (config.usingDisplay()) display.displayText(incomingPayload, 64, 34);
    }

    if(strcmp(incomingTopic, config.getQualificationChan()) == 0) {
        Serial.print("Qualification : ");
        Serial.println(incomingPayload);
        if(incomingPayload[0] == '1') {
            if (config.usingDisplay()) display.displayText("Authorized", 64, 46);
            Piezo::bip(3);

            if (config.getActuator() == DOOR) verifyAndManageDoor();
            else if (config.getActuator() == LASER) verifyAndManageLaser();
            else if (config.getActuator() == PRINTER) verifyAndManagePrinter();
            else if (config.getActuator() == EMBROIDERER) verifyAndManageEmbroiderer();

            log();

        } else {
            if (config.usingDisplay()) display.displayText("Not authorized", 64, 46);
            Piezo::bip(1);
            Piezo::longBip(1);
        }
    }

    if(strcmp(incomingTopic, config.getPointsChan()) == 0) {
        Serial.print("Nombre de points : ");
        Serial.println(incomingPayload);
        if (config.usingDisplay()) display.displayText(incomingPayload, 64, 52);
    }
}

void verifyAndManageLaser() {
    if(!laser.isUsed()) {
        laser.prepare(lastTag);
    } else {
        Piezo::longBip(2);
    }
}

void verifyAndManagePrinter() {
    if(!printer.isUsed()) {
        printer.prepare(lastTag);
    } else {
        Piezo::longBip(2);
    }
}

void verifyAndManageDoor() {
    if (door.getState() == LOCKED) {
        Serial.println("Unlocking door");
        door.unLock();
        Serial.println("Door unlocked");
    } else {
        Serial.println("Locking door");
        door.lock();
        Serial.println("Door locked");
    }
    delay(1000);
}

void verifyAndManageEmbroiderer() {
    if(!embroiderer.isUsed()) {
        embroiderer.enable(lastTag);
    } else if (embroiderer.isUsedByThisUser(lastTag)) {
        embroiderer.disable();
        Serial.print("Brodeuse utilisée avec ");
        Serial.print(embroiderer.getNbPoints());
        Serial.println(" points");
    } else {
        Piezo::longBip(2);
    }
}

void log() {
    publishMQTT(lastTag, LOG_CHAN);
}

#endif