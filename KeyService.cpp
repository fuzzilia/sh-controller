#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

#include "KeyService.h"

BLE2902* inputReportCccd();
BLE2902* dummyInputReportCccd();
// void initAdvertising(BLEServer *bleServer, BLEHIDDevice *hid);

const uint8_t reportMapData[] = {
    0x05, 0x0c,
    0x09, 0x01,
    0xa1, 0x01,
    0x85, 0x01,
    0x19, 0x00,
    0x2a, 0x3c, 0x02,
    0x15, 0x00,
    0x26, 0x3c, 0x02,
    0x95, 0x01,
    0x75, 0x10,
    0x81, 0x00,
    0xc0,
    0x05, 0x01,
    0x09, 0x06,
    0xa1, 0x01,
    0x85, 0x02,
    0x05, 0x07,
    0x19, 0xe0,
    0x29, 0xe7,
    0x15, 0x00,
    0x25, 0x01,
    0x75, 0x01,
    0x95, 0x08,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x08,
    0x25, 0x65,
    0x19, 0x00,
    0x29, 0x65,
    0x81, 0x00,
    0xc0
};

class ReportMapCallbacks : public BLECharacteristicCallbacks {
	void onRead(BLECharacteristic* pCharacteristic) override {
        Serial.println("onRead");
    }
};

BLEHIDDevice *hid = NULL;
BLECharacteristic* dummyInputReport;
BLECharacteristic* inputReport;

BLE2902* inputReportCccd() {
    return (BLE2902*)inputReport->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
}

BLE2902* dummyInputReportCccd() {
    return (BLE2902*)inputReport->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
}

void initKeyService(BLEServer* bleServer) {
    hid = new BLEHIDDevice(bleServer);

    dummyInputReport = hid->inputReport(1);
    inputReport = hid->inputReport(2);
    hid->hidInfo(0x00, 0x02);
    hid->reportMap((uint8_t *)reportMapData, sizeof(reportMapData));
    auto c = hid->hidService()->getCharacteristic(BLEUUID((uint16_t)0x2a4b));
    
    BLESecurity *security = new BLESecurity();
    security->setAuthenticationMode(ESP_LE_AUTH_BOND);
    
    hid->startServices();
}

uint8_t inputKeyValue[] = { 0, 0 };

void setKeyToInputValue(Key key) {
    uint8_t modifier = 0;

    inputKeyValue[0] = 
        (key.modifer.control << 0) |
        (key.modifer.shift << 1) |
        (key.modifer.alt << 2) |
        (key.modifer.gui << 3);
    inputKeyValue[1] = key.key;
}

void printSendKey(Key key) {
    Serial.print("send key");
    if (key.modifer.control) { Serial.print("[control]"); }
    if (key.modifer.shift) { Serial.print("[shift]"); }
    if (key.modifer.alt) { Serial.print("[alt]"); }
    if (key.modifer.gui) { Serial.print("[gui]"); }
    Serial.print(key.key);
    Serial.print("\n");
}

void sendKey(Key key) {
    printSendKey(key);
    setKeyToInputValue(key);
    inputReport->setValue((uint8_t *)inputKeyValue, sizeof(inputKeyValue));
    inputReport->notify();
}

void onConnectKeyService() {
    inputReportCccd()->setNotifications(true);
    dummyInputReportCccd()->setNotifications(true);
}

void onDisconnectKeyService() {
    inputReportCccd()->setNotifications(false);
    dummyInputReportCccd()->setNotifications(false);
}
