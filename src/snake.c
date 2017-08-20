#include <cpctelera.h>
#include "grid.h"
#include "snake.h"
#include "sprites.h"


/*
 * Snake
 */

void snake_init(TSnake *snake) {
    snake->dx = -1;
    snake->dy = 0;
    snake->head = 0;
    snake->tail = 0;
    snake->size = 1;
    snake->mask = BIG_SNAKE_MASK;
    snake->nodes[0].x = NUM_COLUMNS / 2;
    snake->nodes[0].y = NUM_ROWS / 2 - 4;
}

void snake_small_init(TSnakeSmall *snake) {
    snake->dx = -1;
    snake->dy = 0;
    snake->head = 0;
    snake->tail = 0;
    snake->size = 1;
    snake->mask = SMALL_SNAKE_MASK;
    snake->nodes[0].x = NUM_COLUMNS / 2;
    snake->nodes[0].y = NUM_ROWS / 2 - 4;
}

void snake_draw_head(TSnake *snake) {
    u8 *ptr;
    const char *sprite;

    ptr = get_tile_ptr(SNAKE_HEAD(snake).x, SNAKE_HEAD(snake).y);
    /* cpct_drawSolidBox(ptr, 0xFF, TILE_WIDTH, TILE_HEIGHT); */
    if (snake->dx) {
        if (snake->dx == 1) {
            sprite = SpriteSnakeHeadRight;
        } else {
            sprite = SpriteSnakeHeadLeft;
        }
    } else {
        if (snake->dy == 1) {
            sprite = SpriteSnakeHeadDown;
        } else {
            sprite = SpriteSnakeHeadUp;
        }
    }
    cpct_drawSprite(sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_draw_body(TSnake *snake) {
    u8 *ptr;
    const char *sprite;

    ptr = get_tile_ptr(SNAKE_HEAD(snake).x, SNAKE_HEAD(snake).y);
    if (snake->dx) {
        sprite = SpriteSnakeBodyHorz;
    } else {
        sprite = SpriteSnakeBodyVert;
    }
    cpct_drawSprite (sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_erase_tail(TSnake *snake) {
    u8 *ptr;

    ptr = get_tile_ptr(SNAKE_TAIL(snake).x, SNAKE_TAIL(snake).y);
    cpct_drawSolidBox(ptr, 0, TILE_WIDTH, TILE_HEIGHT);

}


void snake_update(TSnake *snake) {
    TSnakeNode head;

    head.x = SNAKE_HEAD(snake).x;
    head.y = SNAKE_HEAD(snake).y;

    if (snake->dx) { // horizontal movement
        head.x += snake->dx;
        if (head.x < 0) {
            head.x = NUM_COLUMNS - 1;
        } else if (head.x >= NUM_COLUMNS) {
            head.x = 0;
        }
    } else {
        head.y += snake->dy;
        if (head.y < 0) {
            head.y = NUM_ROWS - 1;
        } else if (head.y >= NUM_ROWS) {
            head.y = 0;
        }
    }
    // NOTE: if the maximum length of the snake is not a power of two an "if"
    // statement is required to work around index wrapping.
    snake->tail = (snake->tail - 1) & snake->mask;
    snake->head = (snake->head - 1) & snake->mask;
    SNAKE_HEAD(snake).x = head.x;
    SNAKE_HEAD(snake).y = head.y;
}


void snake_add_node(TSnake *snake) {
    if (snake->size < snake->mask) {
        snake->tail = (snake->tail + 1) & snake->mask;
        snake->size++;
    }
}
