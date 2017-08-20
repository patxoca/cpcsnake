#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <cpctelera.h>

#define SNAKE_HEAD(snake) (snake)->nodes[(snake)->head]
#define SNAKE_TAIL(snake) (snake)->nodes[(snake)->tail]

/* the implementation requires that the snake's max length MUST be a power of
 * two.
 */

#define BIG_SNAKE_SIZE 256
#define SMALL_SNAKE_SIZE 16
#define BIG_SNAKE_MASK 0xFF
#define SMALL_SNAKE_MASK 0X0F

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
    u8 mask;
    TSnakeNode nodes[BIG_SNAKE_SIZE];
} TSnake;

typedef struct {
    i8 dx;
    i8 dy;
    u8 head;
    u8 tail;
    u8 size;
    u8 mask;
    TSnakeNode nodes[SMALL_SNAKE_SIZE];
} TSnakeSmall;

extern void snake_add_node(TSnake *snake);
extern void snake_draw_body(TSnake *snake);
extern void snake_draw_head(TSnake *snake);
extern void snake_erase_tail(TSnake *snake);
extern void snake_init(TSnake *snake);
extern void snake_update(TSnake *snake);

#endif /* _SNAKE_H_ */
