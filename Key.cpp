#include "Key.h"

KeyModifier::KeyModifier():
    control(0),
    shift(0),
    alt(0),
    gui(0) {}

Key::Key():
    modifer(),
    key(0) {}