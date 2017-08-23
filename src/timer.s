    .area _CODE

int_handler:
    push hl
    ld hl, (#_g_timer)
    inc hl                      ; 16 bits inc
    ld (#_g_timer), hl
    pop hl
    ei
    reti

_timer_setup::
_timer_resume::
    push af
    push hl
    ld a, #0xC3                 ; JP
    ld hl, #int_handler
    di
    ld (#0x0038), a             ; write JP instruction
    ld (#0x0039), hl            ; write address    #0x38
    ei
    pop hl
    pop af
    ret

_timer_reset::
    push af
    xor a
    di
    ld (#_g_timer), a
    ld (#_g_timer + 1), a
    ei
    pop af
    ret

_timer_pause::
    push hl
    ld hl, #0xC9FB              ; C9 = RET, FB = EI
                                ; Z80 is little endian
    di
    ld (#0x0038), hl
    ei
    pop hl
    ret

_timer_step::
    push hl
    ld hl, (#_g_timer)
    inc hl
    ld (#_g_timer), hl
    pop hl
    ret

    .area _DATA
_g_timer::
    .dw 0
