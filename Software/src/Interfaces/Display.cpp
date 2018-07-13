//
// Created by elektron on 19/06/18.
//

#include "Display.h"
#include "image.h"


Display::Display() : ssd1306(0x3c, 13, 16) {
}


void Display::init() {
    ssd1306.init();
    ssd1306.flipScreenVertically();
    ssd1306.setFont(ArialMT_Plain_10);

    this->setBackground("Booting...");
    ssd1306.display();
}

void Display::displayText(char *text) {
    ssd1306.clear();

    ssd1306.setColor(WHITE);
    ssd1306.setTextAlignment(TEXT_ALIGN_CENTER);
    ssd1306.drawString(64, 15, text);

    ssd1306.display();
}

void Display::displayText(char *text, int y) {
    ssd1306.setColor(WHITE);
    ssd1306.setTextAlignment(TEXT_ALIGN_CENTER);
    ssd1306.drawString(64, y, text);

    ssd1306.display();
}

void Display::diplayCenterText(char *text) {
    ssd1306.clear();

    ssd1306.setColor(WHITE);
    ssd1306.setTextAlignment(TEXT_ALIGN_CENTER);
    ssd1306.drawString(64, 15, text);

    ssd1306.display();
}

void Display::displayLogo() {
    ssd1306.clear();
    ssd1306.drawFastImage(10, 3, imageWidth, imageHeight, bitmap);
    ssd1306.display();
}

void Display::displayText(char *text, int x, int y) {
    ssd1306.setColor(WHITE);
    ssd1306.setTextAlignment(TEXT_ALIGN_CENTER);
    ssd1306.drawString(x, y, text);

    ssd1306.display();
}

void Display::eraseTextZone() {
    ssd1306.setColor(BLACK);
    ssd1306.fillRect(0, 15, 128,64-10 );
}

void Display::setBackground(char *ip) {
    ssd1306.setColor(BLACK);
    ssd1306.fillRect(0, 0, 128,64);
    ssd1306.setColor(WHITE);
    ssd1306.setTextAlignment(TEXT_ALIGN_CENTER);
    this->displayText(ip, 80,2);
}

void Display::beginTimer(long timeToWait) {
    isTimerEnabled = true;
    startingTime = millis();
    this->timeToWait = timeToWait;
}

bool Display::checkTimer() {
    if(isTimerEnabled) {
        return millis() - startingTime >= timeToWait;
    } else {
        return false;
    }
}

void Display::stopTimer() {
    isTimerEnabled = false;
}
