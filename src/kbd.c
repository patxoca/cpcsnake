#include <cpctelera.h>
#include "kbd.h"

void kbd_discard_keys(void) {
    do {
        cpct_scanKeyboard_f();
    } while (cpct_isAnyKeyPressed_f());
}

void kbd_wait_any_key(unsigned char discard) {
    if (discard) {
        kbd_discard_keys();
    }
    do {
        cpct_scanKeyboard_f();
    } while (!cpct_isAnyKeyPressed_f());
}
