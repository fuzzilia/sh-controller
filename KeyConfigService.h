#ifndef FUZZILIA_KEY_CONFIG_SERVICE_H
#define FUZZILIA_KEY_CONFIG_SERVICE_H

#include "KeyConfig.h"

void initKeyConfigService(BLEServer *bleServer, const KeyConfig &keyConfig, boolean isReadonly);

#endif // FUZZILIA_KEY_CONFIG_SERVICE_H