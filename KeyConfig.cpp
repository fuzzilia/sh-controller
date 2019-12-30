#include <utility>
#include <Preferences.h>
#include "KeyConfig.h"

const uint8_t controlDataMask = 0x01 << 0;
const uint8_t shiftDataMask = 0x01 << 1;
const uint8_t altDataMask = 0x01 << 2;
const uint8_t guiDataMask = 0x01 << 3;

KeyConfig::KeyConfig() {
    // header (組み合わせボタンなし)
    m_data[0] = 0x00;
    m_data[1] = 0x00;

    // Button1 A
    m_data[2] = 0x00;
    m_data[3] = 0x04;

    // Button2 Shift + B
    m_data[4] = shiftDataMask;
    m_data[5] = 0x05;

    // Button3
    m_data[6] = 0x00;
    m_data[7] = 0x05;
}

KeyConfig::KeyConfig(const uint8_t *data) {
    memcpy(m_data, data, MAX_DATA_SIZE);

    // 組み合わせボタンの数を数える。ナイーブな実装だが、実行回数が少ないのでこのままで。
    for (int i = 0; i < KEY_SIZE; i++) {
        if (data[0] & (0x01 << i)) {
            m_combinationButtonCount++;
        }
    }
    const int bit2KeySize = KEY_SIZE - 8;
    for (int i = 0; i < bit2KeySize; i++) {
        if (data[1] & (0x01 << i)) {
            m_combinationButtonCount++;
        }
    }
}

Key KeyConfig::keyForButtonState(ButtonState state) const {
    Key key;

    int8_t commandKeyNumber = KEY_NUMBER_NONE;
    uint8_t combinationButtonNumber = 0;

    int commandButtonIndex = 0;
    int combinationButtonIndex = 0;
    for (int8_t i = 0; i < KEY_SIZE; i++) {
        if (isCombinationButton(i)) {
            if (state.isPushed(i)) {
                combinationButtonNumber += 0x01 << combinationButtonIndex;
            }
            combinationButtonIndex++;
        } else {
            if (commandKeyNumber == KEY_NUMBER_NONE) {
                if (state.isPushed(i)) { 
                    commandKeyNumber = commandButtonIndex;
                }
                commandButtonIndex++;
            }
        }
    }

    if (commandKeyNumber != KEY_NUMBER_NONE) {
        const auto combinationOffset = (KEY_SIZE - m_combinationButtonCount) * combinationButtonNumber * 2;
        const int baseIndex = HEADER_BYTE + combinationOffset + commandKeyNumber * 2;
        key.key = m_data[baseIndex + 1];
        if (m_data[baseIndex] & controlDataMask) {
            key.modifer.control = 1;
        }
        if (m_data[baseIndex] & shiftDataMask) {
            key.modifer.shift = 1;
        }
        if (m_data[baseIndex] & altDataMask) {
            key.modifer.alt = 1;
        }
        if (m_data[baseIndex] & guiDataMask) {
            key.modifer.gui = 1;
        }
    }

    return key;
}

uint8_t* KeyConfig::data() const {
    return (uint8_t *)m_data;
}

boolean KeyConfig::isCombinationButton(int8_t buttonNumber) const {
    int byteIndex = buttonNumber / 8;
    int bitIndex = buttonNumber % 8;
    return m_data[byteIndex] & (0x01 << bitIndex);
}

uint8_t KeyConfig::commandKeySize() const {
    return KEY_SIZE;
}
