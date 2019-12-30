#include "ButtonState.h"

ButtonState::ButtonState(): m_state(0) {}

void ButtonState::setIsPushed(uint8_t buttonNumber, bool isPushed) {
    if (isPushed) {
        m_state |= 0x01 << buttonNumber;
    } else {
        m_state &= ~(0x01 << buttonNumber);
    }
}

bool ButtonState::isPushed(uint8_t buttonNumber) const {
    return ((m_state >> buttonNumber) & 0x01) == 0x01;
}

bool ButtonState::operator==(const ButtonState &otherState) const {
    return m_state == otherState.m_state;
}

bool ButtonState::operator!=(const ButtonState &otherState) const {
    return m_state != otherState.m_state;
}