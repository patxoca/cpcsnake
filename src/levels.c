#include "collmap.h"
#include "grid.h"
#include "levels.h"
#include "sprites.h"

#define BRICK 0
#define SAND 1
#define END 0xFF

#define REP_HOR 0x80
#define REP_VER 0x00
#define REP_MASK 0x7F

static const u8* const tile_set[] = {
    SpriteBrick,
    SpriteSand
};

/*
 * Cada nivel es una lista de cuartetos que especifican:
 *
 * - la coordenada x. En unidades de la cuadrícula.
 * - la coordenada y. En unidades de la cuadrícula.
 * - el número de repeticiones. El bit maś alto determina si la repetición es
 *   horizontal (REP_HOR) o vertical (REP_VER). Esto permite un máximo de 127
 *   repeticiones.
 * - el tile a pintar.
 *
 * La definición del mapa debe acabar con un byte END.
 *
 */

static const u8 level_classic[] = {
    0,  0, REP_HOR | 20, BRICK,
    0,  1, REP_VER | 17, BRICK,
    19, 1, REP_VER | 17, BRICK,
    0, 18, REP_HOR | 20, BRICK,
    END
};

static const u8 level_torus[] = {
    9, 9, REP_HOR | 2, BRICK,
    END
};

static const u8 level_madtorus[] = {
    9, 0,   REP_VER | 19, BRICK,
    0, 6,   REP_HOR | 9,  BRICK,
    10, 12, REP_HOR | 9, BRICK,
    END
};

static const u8 level_ordeal[] = {
     9,  0, REP_HOR | 2, BRICK,
     9,  3, REP_HOR | 2, BRICK,
     9,  6, REP_HOR | 2, BRICK,
     9,  9, REP_HOR | 2, BRICK,
     9, 12, REP_HOR | 2, BRICK,
     9, 15, REP_HOR | 2, BRICK,
     9, 18, REP_HOR | 2, BRICK,
    END
};

const TLevel g_levels[NUM_LEVELS] = {
    {9, 9,   "Classic", level_classic,  10},
    {9, 4,   "Torus",   level_torus,    20},
    {6, 10,  "Madtor",  level_madtorus, 30},
    {15, 10, "Ordeal",  level_ordeal,   40}
};

void draw_level(const TLevel *level) {
    u8 const *map = level->map;
    u8 *ptr;
    u8 rep;
    u8 x;
    u8 y;
    u8 dx;
    u8 dy;
    u8 tile_index;

    coma_reset();
    while (*map != END) {
        x = *map++;
        y = *map++;
        rep = *map++;
        tile_index = *map++;
        if (rep & REP_HOR) {
            dx = 1;
            dy = 0;
            rep = rep & REP_MASK;
        } else {
            dx = 0;
            dy = 1;
        }
        while (rep--) {
            coma_set(x, y, COMA_FULL);
            ptr = get_tile_ptr(x, y);
            cpct_drawSprite(tile_set[tile_index], ptr, TILE_WIDTH, TILE_HEIGHT);
            x += dx;
            y += dy;
        }
    }
}
