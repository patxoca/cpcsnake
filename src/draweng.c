/* Draw engine.
 */

#include "draweng.h"

typedef struct {
    u8 width;
    u8 height;
    u8 *sprite;
    u8 *vmem;
} TDrawTask;

#define MAX_TASKS 16

static TDrawTask tasks[MAX_TASKS];
static u8 task_count;

void de_init(void) {
    task_count = 0;
}

void de_reset(void) {
    de_init();
}

void de_draw(u8 *sprite, u8 *vmem, u8 width, u8 height) {
    if (task_count < MAX_TASKS) {
        tasks[task_count].width = width;
        tasks[task_count].height = height;
        tasks[task_count].sprite = sprite;
        tasks[task_count].vmem = vmem;
        task_count++;
    }
}

void de_update(void) {
    u8 i;

    for (i = 0; i < task_count; i++) {
        cpct_drawSprite(tasks[i].sprite, tasks[i].vmem, tasks[i].width, tasks[i].height);
    }
    task_count = 0;
}
