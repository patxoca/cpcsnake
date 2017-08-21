#ifndef _TIMER_H_
#define _TIMER_H_

#include <cpctelera.h>

#define TTimer u16

extern volatile TTimer g_timer;

/*
 * Timer senzillo.
 *
 * Funciona incrementado un contador de 16 bits 300 veces por segundo. Esto
 * permite contar hasta aproximadamente 218 segundos antes de volver a cero.
 */

/*
 * Configura la ISR que actualiza el timer. Debe llamarse una vez al principio
 * del programa.
 */
void timer_setup(void);

/*
 * Pone el timer a cero.
 */
void timer_reset(void);

/*
 * Las funciones siguientes pueden utilizarse para ejecutar el programa
 * fotograma a fotograma. La idea es que el bucle del juego utilize el valor
 * del timer para decidir si genera el siguiente fotograma. Una tecla permite
 * pausar/reactivar el timer y otra incrementarlo. Con esto se genera un
 * fotograma cada vez que se pulsa la tecla.
 */

/*
 * Pausa el contador.
 */
void timer_pause(void);

/*
 * Incrementa el contador, si está pausado. Si no está pausado no tiene ningun
 * efecto.
 */
void timer_step(void);

/*
 * Reactiva el contador.
 */
void timer_resume(void);

#endif /* _TIMER_H_ */
