#ifndef FUZZILIA_KEY_CONFIG_LOADER_H
#define FUZZILIA_KEY_CONFIG_LOADER_H

#include "KeyConfig.h"

/**
 * キー設定のストレージへの保存と読み込みを行う関数を定義するクラスです。
 * ESP32-Arduinoに依存した実装になっています。
 */
class KeyConfigLoader {
public:
    static void save(uint8_t *data, size_t size);
    static KeyConfig load();
};

#endif // FUZZILIA_KEY_CONFIG_LOADER_H