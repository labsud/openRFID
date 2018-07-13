//
// Created by elektron on 27/06/18.
//

#ifndef MQTT_ESP32_EVB_DOOR_H
#define MQTT_ESP32_EVB_DOOR_H

/*Defines for connected pins*/
#define RELAY1 32
#define RELAY2 33
#define CURRENT_SENSOR 34

/*Value returned by current sensor when lock is blocked*/
#define TRIGGER_CURRENT_SENSOR 2900

/*To choose direction when turning*/
#define LOCKING 1
#define UNLOCKING 0

/*To verify state with getState*/
#define LOCKED 1
#define UNLOCKED 0

#define NB_ATTEMPT_FOR_LOCKING 1
#define TURNING_DELAY_IF_NOT_BLOCKED 5000

class Door {
public:
    Door();
    void init(int pinReed);
    void turn(bool dir);
    void stop();
    void lock();
    void unLock();
    bool isBlocked();
    bool isDoorClosed();
    bool getState();

private:
    bool isLocked;
    int pinReed;
};


#endif //MQTT_ESP32_EVB_DOOR_H
