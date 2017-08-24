#ifndef _COLLMAP_H_
#define _COLLMAP_H_

#include <cpctelera.h>
#include "grid.h"

#define COMA_EMPTY 0
#define COMA_FULL 1

extern u8 coll_map[NUM_ROWS][NUM_COLUMNS];

void coma_reset(void);
u8 coma_check(u8 x, u8 y);
void coma_set(u8 x, u8 y, u8 value);

#define coma_clear(x, y) coma_set(x, y, COMA_EMPTY)

#endif /* _COLLMAP_H_ */
