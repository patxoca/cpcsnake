#include "collmap.h"

u8 coll_map[NUM_ROWS][NUM_COLUMNS];


void coma_reset(void) {
    cpct_memset(coll_map, COMA_EMPTY, sizeof(coll_map));
}

u8 coma_check(u8 x, u8 y) {
    return coll_map[y][x];
}

void coma_set(u8 x, u8 y, u8 value) {
    coll_map[y][x] = value;
}
