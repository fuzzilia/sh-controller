#ifndef FUZZILIA_KEY_CONFIG_H
#define FUZZILIA_KEY_CONFIG_H

#include <stdint.h>
#include "ButtonState.h"
#include "Key.h"

#define MAX_COMBINATION_BUTTON_SIZE 3

/**
 * どのボタンに対して何のキーが割り付けられているかを記憶し、
 * 与えられたボタンの状態に対して対応するキーを返すクラスです。
 */
class KeyConfig {
public:
    static const size_t KEY_SIZE = 10;
    static const size_t HEADER_BYTE = 2;
    static const size_t MAX_DATA_SIZE = HEADER_BYTE + 56 * 2; // 組み合わせボタン3つの場合が 2^3 * 7 の組み合わせで最大サイズ

private:
    static const int8_t KEY_NUMBER_NONE = -1;
    uint8_t m_data[MAX_DATA_SIZE];
    uint8_t m_combinationButtonCount = 0;

public:
    KeyConfig();
    KeyConfig(const uint8_t *data);

    Key keyForButtonState(ButtonState state) const;
    uint8_t* data() const;

private:
    int8_t firstCommandKeyNumber(ButtonState state) const;
    boolean isCombinationButton(int8_t buttonNumber) const;
    uint8_t commandKeySize() const;
};

#endif // FUZZILIA_KEY_CONFIG_H