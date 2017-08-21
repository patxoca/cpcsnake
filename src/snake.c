#include <cpctelera.h>
#include "grid.h"
#include "snake.h"
#include "sprites.h"


/*
 * Snake
 */

void snake_init(TSnake *snake, i8 x, i8 y) {
    snake->dx = 0;
    snake->dy = 0;
    snake->head = 0;
    snake->tail = 0;
    snake->size = 1;
    snake->mask = BIG_SNAKE_MASK;
    snake->nodes[0].x = x;
    snake->nodes[0].y = y;
}

void snake_draw_head(TSnake *snake) {
    u8 *ptr;
    const char *sprite;
    TSnakeNode *head;

    head = snake_get_head(snake);
    ptr = get_tile_ptr(head->x, head->y);
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
    TSnakeNode *head;

    head = snake_get_head(snake);
    ptr = get_tile_ptr(head->x, head->y);
    if (snake->dx) {
        sprite = SpriteSnakeBodyHorz;
    } else {
        sprite = SpriteSnakeBodyVert;
    }
    cpct_drawSprite(sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_draw_tail(TSnake *snake) {
    u8 *ptr;
    const char *sprite;
    TSnakeNode *tail;
    TSnakeNode *prev_tail;

    tail = snake_get_tail(snake);
    prev_tail = &snake->nodes[(snake->tail - 1) & snake->mask];
    if (tail->x == prev_tail->x) {
        sprite = SpriteSnakeBodyVert;
    } else {
        sprite = SpriteSnakeBodyHorz;
    }
    ptr = get_tile_ptr(tail->x, tail->y);
    cpct_drawSprite(sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_erase_tail(TSnake *snake) {
    u8 *ptr;
    TSnakeNode *tail;

    tail = snake_get_tail(snake);
    ptr = get_tile_ptr(tail->x, tail->y);
    cpct_drawSolidBox(ptr, 0, TILE_WIDTH, TILE_HEIGHT);
}

void snake_update(TSnake *snake) {
    i8 x;
    i8 y;
    TSnakeNode *head;

    head = snake_get_head(snake);
    x = head->x;
    y = head->y;

    if (snake->dx) { // horizontal movement
        x += snake->dx;
        if (x < 0) {
            x = NUM_COLUMNS - 1;
        } else if (x >= NUM_COLUMNS) {
            x = 0;
        }
    }

    if (snake->dy) {
        y += snake->dy;
        if (y < 0) {
            y = NUM_ROWS - 1;
        } else if (y >= NUM_ROWS) {
            y = 0;
        }
    }

    // NOTE: if the maximum length of the snake is not a power of two an "if"
    // statement is required to work around index wrapping.
    snake->tail = (snake->tail - 1) & snake->mask;
    snake->head = (snake->head - 1) & snake->mask;

    head = snake_get_head(snake);
    head->x = x;
    head->y = y;
}

void snake_add_node(TSnake *snake) {
    if (snake->size < snake->mask) {
        snake->tail = (snake->tail + 1) & snake->mask;
        snake->size++;
        // NOTE: as long as the snake grows by eating fruits, one node at a
        // time, it is not required to initialize the newly added node. In
        // order to get to the fruit the snake will move at least one
        // position. That warrants that there will be at least one discarded
        // node in the queue, the previous tail, already initialized, that
        // will be recycled into the new tail.
    }
}

TSnakeNode *snake_get_head(TSnake *snake) {
    return &snake->nodes[snake->head];
}

TSnakeNode *snake_get_tail(TSnake *snake) {
    return &snake->nodes[snake->tail];
}
