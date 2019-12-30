#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "KeyConfigService.h"
#include "KeyConfigLoader.h"

void initAdvertising(BLEServer *bleServer);

const uint8_t keyConfigServiceUuidData[] = 
    { 0x34, 0x21, 0x6d, 0x2d, 0x31, 0xb5, 0x4d, 0x74, 0x9d, 0xc2, 0x98, 0x17, 0xec, 0x3e, 0x6a, 0x74 };
BLEUUID keyConfigServiceUuid((uint8_t*)keyConfigServiceUuidData, sizeof(keyConfigServiceUuidData), true);

const uint8_t keyConfigCharacteristicUuidData[] = 
    { 0x61, 0xb4, 0x1e, 0x42, 0x3c, 0x16, 0x4b, 0xfc, 0x9c, 0x8a, 0xc5, 0x3b, 0x34, 0x05, 0x96, 0x9b };
BLEUUID keyConfigCharacteristicUuid((uint8_t*)keyConfigCharacteristicUuidData, sizeof(keyConfigCharacteristicUuidData), true);

BLEService* keyConfigService = nullptr;
BLECharacteristic* keyConfigCharacteristic = nullptr;

class KeyConfigCallbacks : public BLECharacteristicCallbacks {
public:
	void onWrite(BLECharacteristic* pCharacteristic) override {
        Serial.println("onWrite");
        Serial.print("Size : ");
        auto value = pCharacteristic->getValue();
        Serial.print((int)value.size());
        Serial.println("");
        KeyConfigLoader::save((uint8_t *)value.data(), value.size());
    }
};

void initKeyConfigService(BLEServer *bleServer, const KeyConfig &keyConfig, boolean isReadonly) {
    keyConfigService = bleServer->createService(keyConfigServiceUuid);
    uint32_t properties = BLECharacteristic::PROPERTY_READ;
    if (!isReadonly) {
        properties = properties | BLECharacteristic::PROPERTY_WRITE;
    }
    keyConfigCharacteristic = keyConfigService->createCharacteristic(keyConfigCharacteristicUuid, properties);
    keyConfigCharacteristic->setCallbacks(new KeyConfigCallbacks());
    keyConfigCharacteristic->setValue(keyConfig.data(), KeyConfig::MAX_DATA_SIZE);

    keyConfigService->start();
}
