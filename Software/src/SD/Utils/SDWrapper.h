//
// Created by elektron on 22/06/18.
//

#ifndef SD_ESP32_SDWRAPPER_H
#define SD_ESP32_SDWRAPPER_H

#include "SD_MMC.h"
#include "Arduino.h"

class SDWrapper {
public:
    SDWrapper();
    bool begin();
    void end();
    void listDir(const char* dirname, uint8_t levels);
    void createDir(const char * path);
    void removeDir(const char* path);
    void readFile(const char *path, char *string);
    void readLine(const char* path);
    void writeFile(const char* path, const char* message);
    void appendFile(const char* path, const char* message);
    void deleteFile(const char* path);
    void renameFile(const char* path1, const char* path2);
    void testFileIO(const char* path);

private:
};


#endif //SD_ESP32_SDWRAPPER_H
