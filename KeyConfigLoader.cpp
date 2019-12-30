#include <Arduino.h>
#include <utility>
#include <Preferences.h>
#include "KeyConfigLoader.h"

Preferences preferences;
const char *ApplicationKey = "SH-Config";
const char *ConfigKey = "config";
const char *CurrentKey = "current";

void printBuffer(uint8_t *data, size_t size) {
    Serial.print("size=");
    Serial.print((unsigned int)size);
    Serial.print(" data=[");
    for (int i = 0; i < size; i++) {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println("]");
}

void KeyConfigLoader::save(uint8_t *data, size_t size) {
    preferences.begin(ApplicationKey, false);
    preferences.putBytes(ConfigKey, data, size);
    preferences.putChar(CurrentKey, 1);
    preferences.end();
    Serial.println("SaveConfig");
    printBuffer(data, size);
}

KeyConfig KeyConfigLoader::load() {
    preferences.begin(ApplicationKey, true);
    if (preferences.getChar(CurrentKey) > 0) {
        uint8_t buffer[KeyConfig::MAX_DATA_SIZE];
        preferences.getBytes(ConfigKey, buffer, KeyConfig::MAX_DATA_SIZE);
        Serial.println("LoadConfig");
        printBuffer(buffer, KeyConfig::MAX_DATA_SIZE);
        preferences.end();
        return KeyConfig(buffer);
    } else {
        preferences.end();
        return KeyConfig();
    }
}