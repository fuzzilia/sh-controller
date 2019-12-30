#include "ButtonManager.h"

ButtonManager::ButtonManager(
    uint8_t buttonPins[],
    size_t buttonPinSize,
    IsOnFunc isOnFunc,
    ChangeKeyFunc changeKey
):
    m_buttonPins(new uint8_t[buttonPinSize]),
    m_buttonPinSize(buttonPinSize),
    m_keyConfig(),
    m_isOnFunc(isOnFunc),
    m_changeKey(changeKey)
{
    memcpy(m_buttonPins.get(), buttonPins, buttonPinSize);
}

void ButtonManager::checkButtonState() {
    auto nextState = m_state;
    for (int i = 0; i < m_buttonPinSize; i++) {
        const auto pin = m_buttonPins[i];
        nextState.setIsPushed(i, m_isOnFunc(pin));
    }

    if (nextState != m_state) {
        m_changeKey(m_keyConfig.keyForButtonState(nextState));
        m_state = nextState;
    }
}

void ButtonManager::setKeyConfig(const KeyConfig& keyConfig) {
    m_keyConfig = keyConfig;
}
