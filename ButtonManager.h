#ifndef FUZZILIA_BUTTON_MANAGER_H
#define FUZZILIA_BUTTON_MANAGER_H

#include <stdint.h>
#include <memory>

#include "ButtonState.h"
#include "Key.h"
#include "KeyConfig.h"

typedef bool (* IsOnFunc)(uint8_t pin);
typedef void (* ChangeKeyFunc)(Key key);

/**
 * ボタンの状態を読み取り、いつ何のキーコードを送信するかをコントロールするクラスです。
 */
class ButtonManager {
    std::unique_ptr<uint8_t[]> m_buttonPins;
    size_t m_buttonPinSize;
    KeyConfig m_keyConfig;
    IsOnFunc m_isOnFunc;
    ChangeKeyFunc m_changeKey;
    ButtonState m_state;

public:
    ButtonManager(uint8_t buttonPins[], size_t buttonPinSize, IsOnFunc isOnFunc, ChangeKeyFunc changeKey);
    void checkButtonState();
    void setKeyConfig(const KeyConfig&);
};

#endif // FUZZILIA_BUTTON_MANAGER_H