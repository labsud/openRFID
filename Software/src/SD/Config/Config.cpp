//
// Created by elektron on 25/06/18.
//

#include "Config.h"

Config::Config() {
}

void Config::init(char *configFile) {
    Serial.begin(115200);
    SDWrapper sd;
    sd.begin();
    Serial.println("Reading SD");
    sd.readFile(configFile, configString);
    sd.end();
    Serial.println("Parsing data");
    INIReader reader(configString);


    isUsedWifi = strcmp(reader.Get("network", "connection", "UNKNOWN").c_str(), "wifi") == 0;
    if (isUsedWifi) {
        strcpy(ssid, reader.Get("network", "ssid", "UNKNOWN").c_str());
        strcpy(wifiPassword, reader.Get("network", "password", "UNKNOWN").c_str());
    }

    isStaticIP = strcmp(reader.Get("network", "mode", "UNKNOWN").c_str(), "static") == 0;
    if (isStaticIP) {
        strcpy(ip, reader.Get("network", "ip", "UNKNOWN").c_str());
        strcpy(subnet, reader.Get("network", "subnet", "UNKNOWN").c_str());
        strcpy(gateway, reader.Get("network", "gateway", "UNKNOWN").c_str());
        strcpy(dns, reader.Get("network", "dns", "UNKNOWN").c_str());
    }

    strcpy(mqttClientId, reader.Get("mqtt", "clientId", "UNKNOWN").c_str());
    strcpy(mqttServer, reader.Get("mqtt", "server", "UNKNOWN").c_str());
    mqttPort = reader.GetInteger("mqtt", "port", -1);

    strcpy(tagChan, reader.Get("channels", "tag", "UNKNOWN").c_str());
    strcpy(passwordChan, reader.Get("channels", "password", "UNKNOWN").c_str());
    strcpy(qualificationChan, reader.Get("channels", "qualification", "UNKNOWN").c_str());
    strcpy(pointsChan, reader.Get("channels", "points", "UNKNOWN").c_str());
    strcpy(nameChan, reader.Get("channels", "name", "UNKNOWN").c_str());
    strcpy(surnameChan, reader.Get("channels", "surname", "UNKNOWN").c_str());

    strcpy(masterPassword, reader.Get("utils", "password", "UNKNOWN").c_str());
    strcpy(masterTag, reader.Get("utils", "tag", "UNKNOWN").c_str());
    isUsedDisplay = (reader.GetInteger("utils", "display", 0) != 0);
    isUsedKeypad = (reader.GetInteger("utils", "keypad", 0) != 0);

    strcpy(actuator, reader.Get("actuator", "name", "NONE").c_str());
    strcpy(orderChan, reader.Get("actuator", "chan", "NONE").c_str());

    ilsPin = reader.GetInteger("actuator", "ils", -1);
    hallPin = reader.GetInteger("actuator", "hall", -1);
    switchPin = reader.GetInteger("actuator", "switch", -1);
}

void Config::getConfigString() {
    Serial.println("Config file :");
    Serial.println(configString);
}

void Config::getConfig() {
    Serial.println();
    if (isUsedWifi) {
        Serial.println("Utilisation du Wifi : oui");
        Serial.print("SSID Wifi : ");
        Serial.println(ssid);
        Serial.print("Mot de passe Wifi : ");
        Serial.println(wifiPassword);
    } else {
        Serial.println("Connexion en filaire : oui");
    }

    Serial.println();

    if (isStaticIP) {
        Serial.println("Configuration d'IP statique");
        Serial.print("Adresse IP : ");
        Serial.println(ip);
        Serial.print("Masque de sous-réseau : ");
        Serial.println(subnet);
        Serial.print("Passerelle par défaut : ");
        Serial.println(gateway);
        Serial.print("DNS : ");
        Serial.println(dns);
    } else {
        Serial.println("Configuration par DHCP");
    }

    Serial.println();
    Serial.print("ID du client MQTT : ");
    Serial.println(mqttClientId);
    Serial.print("Serveur MQTT : ");
    Serial.println(mqttServer);
    Serial.print("Port du serveur : ");
    Serial.println(mqttPort);

    Serial.println();
    Serial.print("Canal pour envoi du tag : ");
    Serial.println(tagChan);
    Serial.print("Canal pour envoi du mot de passe : ");
    Serial.println(passwordChan);
    Serial.print("Canal pour réception des autorisations : ");
    Serial.println(qualificationChan);
    Serial.print("Canal pour réception du nombre de points restants : ");
    Serial.println(pointsChan);
    Serial.print("Canal pour réception du nom : ");
    Serial.println(nameChan);
    Serial.print("Canal pour réception du prénom : ");
    Serial.println(surnameChan);

    Serial.println();
    Serial.print("Master password : ");
    Serial.println(masterPassword);
    Serial.print("Master tag : ");
    Serial.println(masterTag);
    Serial.println();

    Serial.print("Machine utilisée : ");
    Serial.println(actuator);
    if (strcmp(actuator, "laser") == 0) {
        Serial.print("Canal pour envoi des ordres au laser : ");
        Serial.println(orderChan);
    }
    
    Serial.println();
    Serial.print("Utilisation de l'écran : ");
    if (isUsedDisplay) Serial.println("oui");
    else Serial.println("non");
    Serial.print("Utilisation du clavier : ");
    if (isUsedKeypad) Serial.println("oui");
    else Serial.println("non");

    Serial.println();
    Serial.print("GPIO ILS : ");
    Serial.println(ilsPin);
    Serial.print("GPIO hall : ");
    Serial.println(hallPin);
    Serial.print("GPIO contacteur : ");
    Serial.println(switchPin);
}


char *Config::getIp() {
    return ip;
}

char *Config::getSubnet() {
    return subnet;
}

char *Config::getGateway() {
    return gateway;
}

char *Config::getDns() {
    return dns;
}

char *Config::getSsid() {
    return ssid;
}

char *Config::getWifiPassword() {
    return wifiPassword;
}

char *Config::getServer() {
    return mqttServer;
}

int Config::getPort() {
    return mqttPort;
}

char *Config::getClientId() {
    return mqttClientId;
}

char *Config::getTagChan() {
    return tagChan;
}

char *Config::getPasswordChan() {
    return passwordChan;
}

char *Config::getQualificationChan() {
    return qualificationChan;
}

char *Config::getPointsChan() {
    return pointsChan;
}

char *Config::getNameChan() {
    return nameChan;
}

char *Config::getSurnameChan() {
    return surnameChan;
}

char *Config::getMasterPassword() {
    return masterPassword;
}

char *Config::getMasterTag() {
    return masterTag;
}

int Config::getActuator() {
    if (strcmp(actuator, "laser") == 0) return LASER;
    else if (strcmp(actuator, "door") == 0) return DOOR;
    else if (strcmp(actuator, "printer") == 0) return PRINTER;
    else if (strcmp(actuator, "embroiderer") == 0) return EMBROIDERER;
}

bool Config::usingDisplay() {
    return isUsedDisplay;
}

bool Config::usingKeypad() {
    return isUsedKeypad;
}

bool Config::usingWifi() {
    return isUsedWifi;
}

bool Config::usingStaticIP() {
    return isStaticIP;
}

int Config::getIlsPin() {
    return ilsPin;
}

int Config::getHallPin() {
    return hallPin;
}

int Config::getSwitchPin() {
    return switchPin;
}
