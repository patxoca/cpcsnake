#include "kbd.h"

/*       _   _ _     */
/*  _  _| |_(_) |___ */
/* | || |  _| | (_-< */
/*  \_,_|\__|_|_/__/ */


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


/*  _               _                   __          */
/* | |_____ _  _ __| |_____ __ ___ _   / /  _ _ __  */
/* | / / -_) || / _` / _ \ V  V / ' \ / / || | '_ \ */
/* |_\_\___|\_, \__,_\___/\_/\_/|_||_/_/ \_,_| .__/ */
/*          |__/                             |_|    */

/*
 * TODO: se utilizan dos bits por cada tecla, esto permite representar 4
 * estados. La implementación actual solo necesita 3 estados. Probablemente
 * podria añadirse un cuarto estado KEYDOWN que se active durante un ciclo en
 * las transiciones UP -> DOWN.
 */

static u8 kbd_down_status[10];
static u8 kbd_up_status[10];


void kbd_init(void) {
    cpct_memset(kbd_down_status, 0, sizeof(kbd_down_status));
    cpct_memset(kbd_up_status, 0, sizeof(kbd_up_status));
}

void kbd_update(cpct_keyID *keys) {
    u8 block;
    u8 mask;

    for (; *keys; keys++) {
        block = *keys & 0xFF;
        mask = *keys >> 8;
        if (cpct_keyboardStatusBuffer[block] & mask) {
            kbd_down_status[block] |= mask;      /* set down */
            kbd_up_status[block] &= ~mask;       /* clear up */
        } else {
            if (kbd_down_status[block] & mask) { /* if down set */
                kbd_up_status[block] |= mask;    /* set up */
            } else {                             /* else */
                kbd_up_status[block] &= ~mask;   /* clear up */
            }
            kbd_down_status[block] &= ~mask;     /* clear down */
        }
    }
}

u8 kbd_test_keyup(cpct_keyID key) __z88dk_fastcall {
    // TODO: pide a gritos, ensamblador!!!
    u8 block = key & 0xFF;
    u8 mask = key >> 8;

    return (kbd_up_status[block] & mask) != 0;
}
