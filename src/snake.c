#include <cpctelera.h>
#include "grid.h"
#include "snake.h"
#include "sprites.h"

TSnake snake;

/*
 * Snake
 */

void snake_init(void) {
    snake.dx = -1;
    snake.dy = 0;
    snake.head = 0;
    snake.tail = 0;
    snake.size = 1;
    snake.nodes[0].x = NUM_COLUMNS / 2;
    snake.nodes[0].y = NUM_ROWS / 2 - 4;
}

void snake_draw_head(void) {
    u8 *ptr;
    const char *sprite;

    ptr = get_tile_ptr(SNAKE_HEAD.x, SNAKE_HEAD.y);
    /* cpct_drawSolidBox(ptr, 0xFF, TILE_WIDTH, TILE_HEIGHT); */
    if (snake.dx) {
        if (snake.dx == 1) {
            sprite = SpriteSnakeHeadRight;
        } else {
            sprite = SpriteSnakeHeadLeft;
        }
    } else {
        if (snake.dy == 1) {
            sprite = SpriteSnakeHeadDown;
        } else {
            sprite = SpriteSnakeHeadUp;
        }
    }
    cpct_drawSprite(sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_draw_body(void) {
    u8 *ptr;
    const char *sprite;

    ptr = get_tile_ptr(SNAKE_HEAD.x, SNAKE_HEAD.y);
    if (snake.dx) {
        sprite = SpriteSnakeBodyHorz;
    } else {
        sprite = SpriteSnakeBodyVert;
    }
    cpct_drawSprite (sprite, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void snake_erase_tail(void) {
    u8 *ptr;

    ptr = get_tile_ptr(SNAKE_TAIL.x, SNAKE_TAIL.y);
    cpct_drawSolidBox(ptr, 0, TILE_WIDTH, TILE_HEIGHT);

}


void snake_update(void) {
    TSnakeNode head;

    head.x = SNAKE_HEAD.x;
    head.y = SNAKE_HEAD.y;

    if (snake.dx) { // horizontal movement
        head.x += snake.dx;
        if (head.x < 0) {
            head.x = NUM_COLUMNS - 1;
        } else if (head.x >= NUM_COLUMNS) {
            head.x = 0;
        }
    } else {
        head.y += snake.dy;
        if (head.y < 0) {
            head.y = NUM_ROWS - 1;
        } else if (head.y >= NUM_ROWS) {
            head.y = 0;
        }
    }
    // NOTE: if MAX_SNAKE_LEN != 256 an "if" is required to work around index
    // wrapping
    snake.tail--;
    snake.head--;
    SNAKE_HEAD.x = head.x;
    SNAKE_HEAD.y = head.y;
}


void snake_add_node(void) {
    if (snake.size < MAX_SNAKE_LEN - 1) {
        snake.tail++;
        snake.size++;
    }
}
