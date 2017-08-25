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
#include "collmap.h"
#include "draweng.h"
#include "grid.h"
#include "intro.h"
#include "kbd.h"
#include "levels.h"
#include "music.h"
#include "snake.h"
#include "sprites.h"
#include "timer.h"

#define NUM_COLORS 9
#define MODE 0


#if MODE == 0
#define display_zstring(s, p, f, b) cpct_drawStringM0(s, p, f, b)
#elif MODE == 1
#define display_zstring(s, p, f, b) cpct_drawStringM1_f(s, p, f, b)
#else
#define display_zstring(s, p, f, b) cpct_drawStringM2(s, p, f)
#endif

#define clear_screen() cpct_memset(CPCT_VMEM_START, 0, 0x4000)

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

u16 game_delay;

u8 debug_enabled;
u8 debug_paused;
char debug_info[41];

u8 score;
u8 curr_strike;
u8 long_strike;


/*             _      */
/*  __ ___  __| |___  */
/* / _/ _ \/ _` / -_) */
/* \__\___/\__,_\___| */


/*
 * Collision detection
 */

u8 snake_check_collision(void) {
    /* Check if the snake head collides.
     */
    TSnakeNode *head;

    head = snake_get_head(&snake);
    return (coma_check(head->x, head->y) != COMA_EMPTY);
}

u8 snake_check_fruit(void) {
    TSnakeNode *head;

    head = snake_get_head(&snake);
    return (head->x == fruit.x) && (head->y == fruit.y);
}


/*
 * Fruit
 */

void fruit_init(void) {
    do {
        fruit.x = cpct_rand8() % NUM_COLUMNS;
        fruit.y = cpct_rand8() % NUM_ROWS;
    } while (coma_check(fruit.x, fruit.y) != COMA_EMPTY);
    // TTL = random value between 10 and 19 seconds. 14 is a factor that
    // adjust the counter according to the value of game_delay (16).
    fruit.ttl = ((cpct_rand8() % 10) + 1) * 14;
}

void fruit_tick(void) {
    fruit.ttl--;
}

void fruit_draw() {
    u8 *ptr;

    ptr = get_tile_ptr(fruit.x, fruit.y);
    de_draw(SpriteRock, ptr, TILE_WIDTH, TILE_HEIGHT);
}

void fruit_erase() {
    u8 *ptr;

    ptr = get_tile_ptr(fruit.x, fruit.y);
    de_draw(SpriteBlack, ptr, TILE_WIDTH, TILE_HEIGHT);
}

/*
 * Keyboard
 */

void game_read_keys(void) {
    cpct_scanKeyboard_f();
    if (!cpct_isAnyKeyPressed_f()) {
        return;
    }
    if (cpct_isKeyPressed(Key_J) && !snake.dx) {
        snake.dx = -1;
        snake.dy = 0;
        return;
    }
    if (cpct_isKeyPressed(Key_L) && !snake.dx) {
        snake.dx = 1;
        snake.dy = 0;
        return;
    }
    if (cpct_isKeyPressed(Key_I) && !snake.dy) {
        snake.dx = 0;
        snake.dy = -1;
        return;
    }
    if (cpct_isKeyPressed(Key_K) && !snake.dy) {
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
        snake_add_node(&snake);
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
    TSnakeNode *head;

    head = snake_get_head(&snake);
    sprintf(
        debug_info, "[%02x %02x %02x]  (%02x %02x)",
        head->x, head->y, snake.size, fruit.x, fruit.y
        );
    display_zstring(debug_info, ptr, 1, 0);
}

void display_u8(u8 n, u8 x, u8 y) {
    u8 *ptr;
    char str[4];

    str[0] = '0' + (n / 100);
    str[1] = '0' + (n / 10) % 10;
    str[2] = '0' + n % 10;
    str[3] = 0;
    ptr = cpct_getScreenPtr(CPCT_VMEM_START, x, y);
    display_zstring(str, ptr, 1, 0);
}

void menu(void) {
    u8 *ptr;

    clear_screen();
    intro();
    ptr = get_pixel_ptr(2 * 4, 136);
    display_zstring("Press any key to", ptr, 1, 0);
    ptr = get_pixel_ptr(2 * 4, 146);
    display_zstring("start a new game", ptr, 1, 0);
    kbd_wait_any_key(1);
    cpct_srand8((u32)g_timer);
}

void display_level_complete(void) {
    u8 *ptr;

    ptr = get_pixel_ptr(2 * 4, 56);
    display_zstring("LEVEL COMPLETED!!", ptr, 1, 0);
    ptr = get_pixel_ptr(2 * 4, 136);
    display_zstring("Press any key to", ptr, 1, 0);
    ptr = get_pixel_ptr(2 * 4, 146);
    display_zstring("start next level", ptr, 1, 0);
    kbd_wait_any_key(1);
}

void display_game_over(void) {
    u8 *ptr;

    ptr = get_pixel_ptr(6 * 4, 56);
    display_zstring("GAME OVER", ptr, 1, 0);
    ptr = get_pixel_ptr(2 * 4, 136);
    display_zstring("Press any key to", ptr, 1, 0);
    ptr = get_pixel_ptr(3 * 4, 146);
    display_zstring("return to menu", ptr, 1, 0);
    kbd_wait_any_key(1);
}

u8 game_loop(const TLevel *level) {
    u8 redraw_score = 1;
    u8 game_over = 0;
    u8 target;
    TSnakeNode *node;

    clear_screen();
    de_reset();
    draw_level(level);
    cpct_setBorder(1);
    cpct_akp_musicInit(G_Menu);

    snake_init(&snake, level->x, level->y);
    fruit_init();
    fruit_draw();
    timer_reset();
    target = level->target;

    while (1) {
        game_read_keys();
        if (g_timer > game_delay) {
            read_keyboard_debug();
            if (debug_enabled) {
                show_debug();
            }
            fruit_tick();
            if (fruit.ttl == 0) {
                curr_strike = 0;
                redraw_score = 1;
                fruit_erase();
                fruit_init();
                fruit_draw();
            }
            // overwrites the head with a body node/sprite
            node = snake_get_head(&snake);
            coma_set(node->x, node->y, COMA_FULL);
            snake_draw_body(&snake);
            // erases the tail
            node = snake_get_tail(&snake);
            coma_clear(node->x, node->y);
            snake_erase_tail(&snake);
            snake_update(&snake);
            snake_draw_head(&snake);
            if (snake_check_collision()) {
                game_over = 1;
                break;
            }
            if (snake_check_fruit()) {
                score++;
                curr_strike++;
                if (score == target) {
                    break;
                }
                redraw_score = 1;
                snake_add_node(&snake);
                // mark the new tail so that the fruit won't be put there.
                node = snake_get_tail(&snake);
                coma_set(node->x, node->y, COMA_FULL);
                fruit_init();
                fruit_draw();
            }
            if (redraw_score) {
                if (curr_strike > long_strike) {
                    long_strike = curr_strike;
                }
                display_u8(score, 11, 1);
                display_u8(curr_strike, 34, 1);
                display_u8(long_strike, 57, 1);
                redraw_score = 0;
            }
            timer_reset();
        }
        cpct_waitVSYNC();
        /* cpct_setBorder(HW_RED); */
        cpct_akp_musicPlay();
        de_update();
        /* cpct_setBorder(HW_GREEN); */
    }

    cpct_setBorder(2);
    cpct_akp_stop();

    return game_over;
}

void game(void) {
    u8 level;
    u8 game_over = 0;

    for (level = 0; (level < NUM_LEVELS) && !game_over; level++) {
        score = 0;
        curr_strike = 0;
        long_strike = 0;
        game_over = game_loop(&g_levels[level]);
        if (game_over) {
            display_game_over();
        } else {
            display_level_complete();
        }
    }
}

void main(void) {
    game_delay = 16;

    debug_enabled = 0;
    debug_paused = 0;

    cpct_setBorder(0);
    cpct_fw2hw(palette, NUM_COLORS);
    cpct_setPalette(palette, NUM_COLORS);
    cpct_setVideoMode(MODE);
    de_init();
    timer_setup();

    while (1) {
        menu();
        game();
    }
}
