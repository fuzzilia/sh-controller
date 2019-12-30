#ifndef FUZZILIA_KEY_H
#define FUZZILIA_KEY_H

#include <stdint.h>

struct KeyModifier {
    uint8_t control: 1;
    uint8_t shift: 1;
    uint8_t alt: 1;
    uint8_t gui: 1;

    KeyModifier();
};

struct Key {
    KeyModifier modifer;
    uint8_t key;

    Key();
};

#endif // FUZZILIA_KEY_H