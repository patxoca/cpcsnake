#include <cpctelera.h>
#include "grid.h"
#include "sprites.h"

#define BRICK 0
#define SAND 1

#define END 0xFF

#define NUM_LEVELS 1


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
    10, 9, SAND,
    END
};

static const u8* const levels[] = {
    level_01
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
