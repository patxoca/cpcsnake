#ifndef _GRID_H_
#define _GRID_H_

/* sizes, in bytes */
#define TILE_WIDTH 4
#define TILE_HEIGHT 10
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 200

#define NUM_COLUMNS SCREEN_WIDTH / TILE_WIDTH
#define NUM_ROWS (SCREEN_HEIGHT / TILE_HEIGHT - 1)

#define get_pixel_ptr(x, y) cpct_getScreenPtr(CPCT_VMEM_START, x, y)
#define get_tile_ptr(x, y) get_pixel_ptr((x) * TILE_WIDTH, ((y) + 1) * TILE_HEIGHT)
#define get_tile_center_ptr(x, y) get_pixel_ptr((x) * TILE_WIDTH + TILE_WIDTH / 2, ((y) + 1) * TILE_HEIGHT + TILE_HEIGHT / 2)

#endif /* _GRID_H_ */
