#include "collmap.h"
#include "grid.h"
#include "levels.h"
#include "sprites.h"

#define BRICK 0
#define SAND 1

#define END 0xFF


static const u8* const tile_set[] = {
    SpriteBrick,
    SpriteSand
};

/*
 * Each level is a list of triples specifying the coordinates, in grid units,
 * of the element and the element. The list ends with a 0xFF byte.
 */

static const u8 level_00[] = {
     0, 0, BRICK,
     1, 0, BRICK,
     2, 0, BRICK,
     3, 0, BRICK,
     4, 0, BRICK,
     5, 0, BRICK,
     6, 0, BRICK,
     7, 0, BRICK,
     8, 0, BRICK,
     9, 0, BRICK,
     10, 0, BRICK,
     11, 0, BRICK,
     12, 0, BRICK,
     13, 0, BRICK,
     14, 0, BRICK,
     15, 0, BRICK,
     16, 0, BRICK,
     17, 0, BRICK,
     18, 0, BRICK,
     19, 0, BRICK,

     0, 1, BRICK, 19, 1, BRICK,
     0, 2, BRICK, 19, 2, BRICK,
     0, 3, BRICK, 19, 3, BRICK,
     0, 4, BRICK, 19, 4, BRICK,
     0, 5, BRICK, 19, 5, BRICK,
     0, 6, BRICK, 19, 6, BRICK,
     0, 7, BRICK, 19, 7, BRICK,
     0, 8, BRICK, 19, 8, BRICK,
     0, 9, BRICK, 19, 9, BRICK,
     0, 10, BRICK, 19, 10, BRICK,
     0, 11, BRICK, 19, 11, BRICK,
     0, 12, BRICK, 19, 12, BRICK,
     0, 13, BRICK, 19, 13, BRICK,
     0, 14, BRICK, 19, 14, BRICK,
     0, 15, BRICK, 19, 15, BRICK,
     0, 16, BRICK, 19, 16, BRICK,
     0, 17, BRICK, 19, 17, BRICK,

     0, 18, BRICK,
     1, 18, BRICK,
     2, 18, BRICK,
     3, 18, BRICK,
     4, 18, BRICK,
     5, 18, BRICK,
     6, 18, BRICK,
     7, 18, BRICK,
     8, 18, BRICK,
     9, 18, BRICK,
     10, 18, BRICK,
     11, 18, BRICK,
     12, 18, BRICK,
     13, 18, BRICK,
     14, 18, BRICK,
     15, 18, BRICK,
     16, 18, BRICK,
     17, 18, BRICK,
     18, 18, BRICK,
     19, 18, BRICK,

    END
};

static const u8 level_01[] = {
     9, 9, BRICK,
    10, 9, BRICK,
    END
};

static const u8 level_02[] = {
     9, 0, BRICK,
     9, 3, BRICK,
     9, 6, BRICK,
     9, 9, BRICK,
     9, 12, BRICK,
     9, 15, BRICK,
     9, 18, BRICK,
    END
};

const TLevel g_levels[NUM_LEVELS] = {
    {9, 9,   "Classic", level_00, 10},
    {9, 4,   "Torus",   level_01, 10},
    {15, 10, "Ordeal",  level_02, 40}
};

void draw_level(const TLevel *level) {
    u8 const *map = level->map;
    u8 *ptr;
    u8 x;
    u8 y;
    u8 tile_index;

    coma_reset();
    while (*map != END) {
        x = *map++;
        y = *map++;
        tile_index = *map++;
        coma_set(x, y, COMA_FULL);
        ptr = get_tile_ptr(x, y);
        cpct_drawSprite(tile_set[tile_index], ptr, TILE_WIDTH, TILE_HEIGHT);
    }
}
