#ifndef FUZZILIA_KEY_SERVICE_H
#define FUZZILIA_KEY_SERVICE_H

#include "Key.h"

void initKeyService(BLEServer* bleServer);
void sendKey(Key key);
void onConnectKeyService();
void onDisconnectKeyService();

#endif // FUZZILIA_KEY_SERVICE_H