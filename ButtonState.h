#ifndef FUZZILIA_BUTTON_STATE_H
#define FUZZILIA_BUTTON_STATE_H

#include <stdint.h>

/**
 * 今どのボタンが押されているかを表すクラスです。
 */
class ButtonState {
    /**
     * どのボタンが押されているかを1bit毎に記録します。
     * （0:押されていない 1:押されている）
     */
    uint16_t m_state;
public:
    ButtonState();
    void setIsPushed(uint8_t buttonNumber, bool isPushed = true);
    bool isPushed(uint8_t buttonNumber) const;

    bool operator==(const ButtonState &otherState) const;
    bool operator!=(const ButtonState &otherState) const;
};

#endif // FUZZILIA_BUTTON_STATE_H