//-----------------------------LICENSE NOTICE------------------------------------
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <cpctelera.h>
#include <stdio.h>
#include "grid.h"
#include "levels.h"
#include "music.h"
#include "sprites.h"

#define NUM_COLORS 9
#define MODE 0


/* Having a circular queue 256 entries log has a nice property,
 * incrementing/decrementing the head and tail indexes past the queue limits
 * automatically wraps them, no "if (out_of_bounds) adjust_index" required.
*/
#define MAX_SNAKE_LEN 256

#if MODE == 0
#define display_zstring(s, p, f, b) cpct_drawStringM0(s, p, f, b)
#elif MODE == 1
#define display_zstring(s, p, f, b) cpct_drawStringM1_f(s, p, f, b)
#else
#define display_zstring(s, p, f, b) cpct_drawStringM2(s, p, f)
#endif

#define clear_screen() cpct_memset(CPCT_VMEM_START, 0, 0x4000)

/*  _                      */
/* | |_ _  _ _ __  ___ ___ */
/* |  _| || | '_ \/ -_|_-< */
/*  \__|\_, | .__/\___/__/ */
/*      |__/|_|            */

typedef struct {
    i8 x;
    i8 y;
} TSnakeNode;

typedef struct {
    i8 dx;
    i8 dy;
    i8 updated;
    u8 head;
    u8 tail;
    u8 size;
    TSnakeNode nodes[MAX_SNAKE_LEN];
} TSnake;

typedef struct {
    i8 x;
    i8 y;
    u16 ttl;
} TFruit;

/*       _     _          _                _      _    _         */
/*  __ _| |___| |__  __ _| | __ ____ _ _ _(_)__ _| |__| |___ ___ */
/* / _` | / _ \ '_ \/ _` | | \ V / _` | '_| / _` | '_ \ / -_|_-< */
/* \__, |_\___/_.__/\__,_|_|  \_/\__,_|_| |_\__,_|_.__/_\___/__/ */
/* |___/                                                         */

const unsigned char palette[NUM_COLORS] = {0, 26, 25, 18, 15, 9, 6, 22, 3};

TSnake snake;
TFruit fruit;

volatile u16 timer;
u16 last_move;
u16 game_delay;

u8 debug_enabled;
u8 debug_paused;
char debug_info[41];


/*             _      */
/*  __ ___  __| |___  */
/* / _/ _ \/ _` / -_) */
/* \__\___/\__,_\___| */


void interrupt_handler(void) {
    timer++;
}

/*
 * Cells/board
 */

u8 cell_check_empty(i8 x, i8 y) {
    u8 *ptr;

    ptr = get_tile_center_ptr(x, y);
    return *ptr == 0;
}

/*
 * Snake
 */

void snake_init(void) {
    snake.dx = -1;
    snake.dy = 0;
    snake.updated = 0;
    snake.head = 0;
    snake.tail = 0;
    snake.size = 1;
    snake.nodes[0].x = NUM_COLUMNS / 2;
    snake.nodes[0].y = NUM_ROWS / 2 - 4;
}

#define SNAKE_HEAD snake.nodes[snake.head]
#define SNAKE_TAIL snake.nodes[snake.tail]

void snake_draw_head() {
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

void snake_draw_body() {
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

void snake_erase_tail() {
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
    // if MAX_SNAKE_LEN != 256 an "if" is required to work around index
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

u8 snake_check_collision(void) {
    /* Check if the snake head collides.
     *
     * By the way it works (looking at the video memory) this function must be
     * called after the snake state has been updated but before is has been
     * drawn to the screen, otherwise the head will collide with itself.
     *
     */
    u8 *ptr;

    ptr = get_tile_center_ptr(SNAKE_HEAD.x, SNAKE_HEAD.y);
    return ((*ptr != 0) && ((fruit.x != SNAKE_HEAD.x) || (fruit.y != SNAKE_HEAD.y)));
}

u8 snake_check_fruit(void) {
    return (SNAKE_HEAD.x == fruit.x) && (SNAKE_HEAD.y == fruit.y);
}


/*
 * Fruit
 */

void fruit_init(void) {
    do {
        fruit.x = cpct_rand8() % NUM_COLUMNS;
        fruit.y = cpct_rand8() % NUM_ROWS;
    } while (!cell_check_empty(fruit.x, fruit.y));
    // TTL = random value between
    fruit.ttl = ((cpct_rand8() % 10) * 6 + 10) * 10;
}

void fruit_tick(void) {
    fruit.ttl--;
}

void fruit_draw() {
    u8 *ptr;

    ptr = get_tile_ptr(fruit.x, fruit.y);
    /* cpct_drawSolidBox(ptr, 0x0C, TILE_WIDTH, TILE_HEIGHT); */
    cpct_drawSprite(SpriteRock, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void fruit_erase() {
    u8 *ptr;

    ptr = get_tile_ptr(fruit.x, fruit.y);
    cpct_drawSolidBox(ptr, 0x0, TILE_WIDTH, TILE_HEIGHT);
}

/*
 * Keyboard
 */

void game_read_keys(void) {
    cpct_scanKeyboard_f();
    if (!cpct_isAnyKeyPressed_f()) {
        return;
    }
    if (cpct_isKeyPressed(Key_J) && snake.dy) {
        snake.dx = -1;
        snake.dy = 0;
        return;
    }
    if (cpct_isKeyPressed(Key_L) && snake.dy) {
        snake.dx = 1;
        snake.dy = 0;
        return;
    }
    if (cpct_isKeyPressed(Key_I) && snake.dx) {
        snake.dx = 0;
        snake.dy = -1;
        return;
    }
    if (cpct_isKeyPressed(Key_K) && snake.dx) {
        snake.dx = 0;
        snake.dy = 1;
        return;
    }
}


void read_keyboard_debug(void) {
    cpct_scanKeyboard_f();
    if (!cpct_isAnyKeyPressed_f()) {
        return;
    }
    if (cpct_isKeyPressed(Key_P)) {
        debug_paused = !debug_paused;
    }
    if (cpct_isKeyPressed(Key_D)) {
        debug_enabled = !debug_enabled;
    }
    if (cpct_isKeyPressed(Key_A)) {
        snake_add_node();
    }
    if (cpct_isKeyPressed(Key_W)) {
        if (game_delay < 50) {
            game_delay += 1;
        }
    }
    if (cpct_isKeyPressed(Key_S)) {
        if (game_delay > 1) {
            game_delay -= 1;
        }
    }
}


void show_debug() {
    u8 *ptr = cpct_getScreenPtr(CPCT_VMEM_START, 0, 0);
    sprintf(
        debug_info, "(%02x, %02x)  %02x %04x",
        SNAKE_HEAD.x, SNAKE_HEAD.y, game_delay, fruit.ttl
        );
    display_zstring(debug_info, ptr, 1, 0);
}

void menu(void) {
    u8 *ptr;

    clear_screen();
    ptr = get_pixel_ptr(0, 96);
    display_zstring("  Press any key to", ptr, 1, 0);
    ptr = get_pixel_ptr(0, 106);
    display_zstring("  start a new game", ptr, 1, 0);
    do {
        cpct_scanKeyboard_f();
    } while (!cpct_isAnyKeyPressed_f());
}

void game(void) {
    clear_screen();
    draw_level(0);
    cpct_setBorder(1);
    cpct_akp_musicInit(G_Menu);

    snake_init();
    fruit_init();
    fruit_draw();
    while (1) {
        game_read_keys();
        if (timer - last_move > game_delay) {
            read_keyboard_debug();
            if (debug_enabled) {
                show_debug();
            }
            cpct_waitVSYNC();
            cpct_akp_musicPlay();
            /* cpct_setBorder(2); */
            fruit_tick();
            if (fruit.ttl == 0) {
                fruit_erase();
                fruit_init();
                fruit_draw();
            }
            snake_draw_body();
            snake_erase_tail();
            snake_update();
            if (snake_check_collision()) {
                break;
            }
            snake_draw_head();
            if (snake_check_fruit()) {
                snake_add_node();
                fruit_init();
                fruit_draw();
            }
            snake.updated = 1;
            last_move = timer;
            /* cpct_setBorder(0); */
        }
    }
    cpct_setBorder(2);
    cpct_akp_stop();
}


void main(void) {
    timer = 0;
    last_move = 0;
    game_delay = 16;

    debug_enabled = 0;
    debug_paused = 0;

    cpct_disableFirmware();
    cpct_setBorder(0);
    cpct_fw2hw(palette, NUM_COLORS);
    cpct_setPalette(palette, NUM_COLORS);
    cpct_setVideoMode(MODE);
    cpct_setInterruptHandler(interrupt_handler);

    while (1) {
        menu();
        game();
    }
}
