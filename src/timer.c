#include "timer.h"

volatile TTimer g_timer;
static u8 paused;

// TODO: escribir el código de interrupt_handler y el código que lo activa en
// ensamblador, eliminando el overhead de las llamadas a funciones C.

static void interrupt_handler(void) {
    if (!paused) {
        g_timer++;
    }
}

void timer_setup(void) {
    g_timer = 0;
    paused = 0;
    cpct_setInterruptHandler(interrupt_handler);
}

void timer_reset(void) {
    __asm
        di
    __endasm;

    g_timer = 0;

    __asm
        ei
    __endasm;
}

void timer_pause(void) {
    paused = 1;
}

void timer_step(void) {
    if (paused) {
        g_timer++;
    }
}

void timer_resume(void) {
    paused = 0;
}
