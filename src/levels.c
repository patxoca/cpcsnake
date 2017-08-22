#include <cpctelera.h>
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

static const u8* const levels[NUM_LEVELS] = {
    level_01, level_02
};

void draw_level(u8 n) {
    u8 const *level = levels[n];
    u8 *ptr;
    u8 x;
    u8 y;
    u8 tile_index;

    while (*level != END) {
        x = *level++;
        y = *level++;
        tile_index = *level++;
        ptr = get_tile_ptr(x, y);
        cpct_drawSprite(tile_set[tile_index], ptr, TILE_WIDTH, TILE_HEIGHT);
    }
}
