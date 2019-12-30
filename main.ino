#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLEHIDDevice.h"
#include "KeyService.h"
#include "KeyConfigService.h"
#include "KeyConfigLoader.h"
#include "ButtonManager.h"

#define LED_PIN 32
#define SELECT_PIN 16
uint8_t buttonPins[] = { 5, 19, 18, 17, 13, 12, 14, 15, 4, 16 };
const char* DEVICE_NAME = "SH-Con";

boolean isConfigMode = false;
boolean isConnected = false;
int connectionCount = 0;

void initAdvertising(BLEServer *bleServer);
class SHControllerBLECallbacks : public BLEServerCallbacks {
    virtual void onConnect(BLEServer *pServer) {
        Serial.println("connected");
        isConnected = true;
        connectionCount++;
        if (connectionCount == 1) {
            onConnectKeyService();
            if (isConfigMode) {
                pServer->startAdvertising();
            }
        }
    }

    virtual void onDisconnect(BLEServer* pServer) {
        Serial.println("disconnected");
        connectionCount--;
        if (connectionCount == 0) {
            isConnected = false;
            onDisconnectKeyService();
        }
    }
};

bool pinIsOn(uint8_t pin) {
    // Serial.print("Pin[");
    // Serial.print(pin);
    // Serial.print("] : ");
    bool isOn = digitalRead(pin) == 0;
    // Serial.print(isOn);
    // Serial.print("\n");
    return isOn;
}

ButtonManager buttonManager(buttonPins, sizeof(buttonPins), pinIsOn, sendKey);

void setup() {
    // init serial
    Serial.begin(115200);
    Serial.println("setup start");

    // init pins
    pinMode(LED_PIN, OUTPUT);
    for (int i = 0; i < sizeof(buttonPins); i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    isConfigMode = digitalRead(SELECT_PIN) == LOW;

    // init key config
    auto config = KeyConfigLoader::load();
    buttonManager.setKeyConfig(config);

    // init ble
    BLEDevice::init(DEVICE_NAME);
    BLEServer *bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(new SHControllerBLECallbacks());
    initAdvertising(bleServer);
    initKeyService(bleServer);
    initKeyConfigService(bleServer, config, !isConfigMode);
    bleServer->getAdvertising()->start();
    
    if (isConfigMode) {
        Serial.println("Config Mode");

        // 設定モードにするためのキー押下を通常のキー入力として認識しないため、ボタンが離されるまで待つ。
        while (digitalRead(SELECT_PIN) == LOW) {
            delay(10);
        }
    } else {
        Serial.println("Normal Mode");
    }
    Serial.println("setup finished");
}

void loop() {
    delay(20);
    if (!isConfigMode) {
        buttonManager.checkButtonState();
    }
}

void initAdvertising(BLEServer *bleServer) {
    auto advertising = bleServer->getAdvertising();
    BLEAdvertisementData advertisementData;
    advertisementData.setName(DEVICE_NAME);
    advertising->setAppearance(HID_KEYBOARD);
    advertising->addServiceUUID(BLEUUID((uint16_t) 0x1812));
    advertising->setAdvertisementData(advertisementData);
}