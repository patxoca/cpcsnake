#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cpctelera.h>

#define SNAKE_HEAD(snake) (snake)->nodes[(snake)->head]
#define SNAKE_TAIL(snake) (snake)->nodes[(snake)->tail]

/* snake max length MUST be a power of two
 *
 * Having a circular queue 256 entries log has a nice property,
 * incrementing/decrementing the head and tail indexes past the queue limits
 * automatically wraps them, no "if (out_of_bounds) adjust_index" required.
*/

#define MAX_SNAKE_LEN 256

typedef struct {
    i8 x;
    i8 y;
} TSnakeNode;

typedef struct {
    i8 dx;
    i8 dy;
    u8 head;
    u8 tail;
    u8 size;
    TSnakeNode nodes[MAX_SNAKE_LEN];
} TSnake;

extern void snake_add_node(TSnake *snake);
extern void snake_draw_body(TSnake *snake);
extern void snake_draw_head(TSnake *snake);
extern void snake_erase_tail(TSnake *snake);
extern void snake_init(TSnake *snake);
extern void snake_update(TSnake *snake);

#endif /* _SNAKE_H_ */
