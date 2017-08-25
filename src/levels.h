#ifndef _LEVELS_H_
#define _LEVELS_H_

#include <cpctelera.h>

#define NUM_LEVELS 4

typedef struct {
    u8 x;              // x inicial de la serpiente
    u8 y;              // y inicial de la serpiente
    const char *title; // nombre del nivel
    const u8 *map;     // mapa del nivel
    u8 target;         // nombre de frutas que hay que comer para dar por
                       // finalizado el nivel
} TLevel;


extern const TLevel g_levels[NUM_LEVELS];


void draw_level(const TLevel *level);

#endif /* _LEVELS_H_ */
