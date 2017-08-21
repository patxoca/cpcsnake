/*
 * Animates the intro screen.
 *
 * Five snakes move around the screen until they form the word SNAKE.
 */

#include <cpctelera.h>
#include "snake.h"
#include "timer.h"

// One snake for each letter
#define NUM_SNAKES 5

#define ANIMATION_NUM_STEPS 27

// delay between frames, in 1/300 sec units
#define ANIMATION_DELAY 20

#define foreach_snake(i) for (i = 0; i < NUM_SNAKES; i++ )

/*  _                      */
/* | |_ _  _ _ __  ___ ___ */
/* |  _| || | '_ \/ -_|_-< */
/*  \__|\_, | .__/\___/__/ */
/*      |__/|_|            */


/*
 * Commands recognized by the animation engine.
 */

typedef enum {
    M_UP,
    M_DOWN,
    M_LEFT,
    M_RIGHT,
    M_UP_LEFT,
    M_UP_RIGHT,
    M_DOWN_LEFT,
    M_DOWN_RIGHT,
    M_STOP,
} TAnimationCommand;

/*
 * Initial data for the snakes
 */

typedef struct {
    u8 x; // initial X position, in grid units
    u8 y; // initial Y position, in grid units
    u8 l; // length of the snake. We are using TSnakeSmall, so it's length is
          // limited to SMALL_SNAKE_SIZE nodes.
} TSnakeStartupInfo;

/*
 * Definition of an animations' segment.
 */

typedef struct {
    u8 num_steps;               // repetition count
    TAnimationCommand command;  // animation command
} TAnimationSegment;

/*
 * Animation state.
 */

typedef struct {
    u8 segment_index;  // which segment is executing?
    u8 command;        // animation command
    u8 steps_left;     // remaining steps
    u8 finished;       // animation finished?
} TAnimationState;


/*       _     _          _                _      _    _         */
/*  __ _| |___| |__  __ _| | __ ____ _ _ _(_)__ _| |__| |___ ___ */
/* / _` | / _ \ '_ \/ _` | | \ V / _` | '_| / _` | '_ \ / -_|_-< */
/* \__, |_\___/_.__/\__,_|_|  \_/\__,_|_| |_\__,_|_.__/_\___/__/ */
/* |___/                                                         */

static TSnakeSmall snakes[NUM_SNAKES];

/*
 * Initial position for each snake + snake length, in grid units.
 */

static const TSnakeStartupInfo initial_position[NUM_SNAKES] = {
    {0, 18, 12},  // S
    {4, 18, 14},  // N
    {9, 18, 13},  // A
    {13, 18, 12}, // K
    {19, 18, 13}  // E
};

/*
 * Animation segments for each snake.
 */

static const TAnimationSegment anim_S[] = {
    {14, M_UP},
    {2, M_RIGHT},
    {2, M_UP},
    {2, M_LEFT},
    {2, M_UP},
    {2, M_RIGHT},
    {1, M_STOP}
};

static const TAnimationSegment anim_N[] = {
    {18, M_UP},
    {3, M_DOWN_RIGHT},
    {1, M_DOWN},
    {4, M_UP},
    {1, M_STOP}
};

static const TAnimationSegment anim_A[] = {
    {18, M_UP},
    {2, M_RIGHT},
    {1, M_DOWN},
    {1, M_DOWN_LEFT},
    {1, M_RIGHT},
    {2, M_DOWN},
    {1, M_STOP}
};

static const TAnimationSegment anim_K[] = {
    {18, M_UP},
    {2, M_DOWN},
    {2, M_DOWN_RIGHT},
    {1, M_UP_LEFT},
    {1, M_UP_RIGHT},
    {1, M_STOP}
};

static const TAnimationSegment anim_E[] = {
    {14, M_UP},
    {2, M_LEFT},
    {1, M_UP},
    {1, M_UP_RIGHT},
    {1, M_RIGHT},
    {2, M_LEFT},
    {2, M_UP},
    {2, M_RIGHT},
    {1, M_STOP}
};

/*
 * Animations put togheter.
 */

static const TAnimationSegment* const animations[NUM_SNAKES] = {
   anim_S, anim_N, anim_A, anim_K, anim_E
};

/*
 * State of the animation for each snake.
 */

static TAnimationState animation_state[NUM_SNAKES];


/*             _      */
/*  __ ___  __| |___  */
/* / _/ _ \/ _` / -_) */
/* \__\___/\__,_\___| */


/*
 * Initialize the snakes from the initial data.
 */

static void snakes_init(void) {
    u8 i;
    u8 j;
    TSnakeSmall *snk;

    foreach_snake(i) {
        snk = &snakes[i];
        j = snk->size = initial_position[i].l;
        while (j) {
            snk->nodes[j - 1].x = initial_position[i].x;
            snk->nodes[j - 1].y = initial_position[i].y;
            j--;
        }
        snk->mask = SMALL_SNAKE_MASK;
        snk->head = 0;
        snk->tail = initial_position[i].l - 1;
        snk->dx = 0;
        snk->dy = 0;
    }
}

/*
 * Initialize the animation state for each snake.
 */

static void animation_init(void) {
    u8 i;

    foreach_snake(i) {
        animation_state[i].segment_index = 0xFF; // -1
        animation_state[i].command = 0;
        animation_state[i].steps_left = 0;
        animation_state[i].finished = 0;
    }
}

/*
 * Run an step and display a frame.
 */

static void animation_step(void) {
    u8 i;
    TSnakeSmall *snk;
    const TAnimationSegment *frm;

    foreach_snake(i) {
        if (!animation_state[i].finished) {
            if (!animation_state[i].steps_left) {
                // no steps left, fetch a new segment
                animation_state[i].segment_index++;
                frm = &(animations[i][animation_state[i].segment_index]);
                animation_state[i].command = frm->command;
                animation_state[i].steps_left = frm->num_steps;
            }

            snk = &snakes[i];

            animation_state[i].steps_left--;

            // update snake speed and/or animation state acording to the
            // current command.

            switch (animation_state[i].command) {
            case M_STOP:
                animation_state[i].finished = 1;
                snk->dx = 0;
                snk->dy = 0;
                break;

            case M_LEFT:
                snk->dx = -1;
                snk->dy = 0;
                break;

            case M_RIGHT:
                snk->dx = 1;
                snk->dy = 0;
                break;

            case M_UP:
                snk->dx = 0;
                snk->dy = -1;
                break;

            case M_DOWN:
                snk->dx = 0;
                snk->dy = 1;
                break;

            case M_UP_RIGHT:
                snk->dx = 1;
                snk->dy = -1;
                break;

            case M_UP_LEFT:
                snk->dx = -1;
                snk->dy = -1;
                break;

            case M_DOWN_RIGHT:
                snk->dx = 1;
                snk->dy = 1;
                break;

            case M_DOWN_LEFT:
                snk->dx = -1;
                snk->dy = 1;
                break;
            }

            // update the screen

            snake_erase_tail((TSnake *)snk);
            snake_draw_body((TSnake *)snk);
            snake_update((TSnake *)snk);
            snake_draw_head((TSnake *)snk);
        }
    }
}


/*
 * Display the intro.
 */

void intro(void) {
    u8 i;

    snakes_init();
    animation_init();
    for (i = 0; i < ANIMATION_NUM_STEPS; i++) {
        cpct_waitVSYNC();
        animation_step();
        timer_reset();
        while (g_timer < ANIMATION_DELAY) ;
    }

}
