//
// Created by elektron on 19/06/18.
//

#include <esp_wifi.h>
#include "Wifi.h"

Wifi::Wifi(){
}

void Wifi::init(char *ssid, char *password) {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void Wifi::init(char *ssid, char *password, IPAddress ip, IPAddress subnet, IPAddress gateway, IPAddress dns) {
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.config(ip, dns, gateway, subnet);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

String Wifi::getIP() {
    return WiFi.localIP().toString();
}

WiFiClient& Wifi::getClient() {
    return espWifiClient;
}
