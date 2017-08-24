#ifndef _DRAWENG_H_
#define _DRAWENG_H_

/*
 * Draw engine.
 *
 * El draw engine se encarga de pintar los sprites en pantalla.
 *
 * La lógica de juego no pinta los sprites directamente, en su lugar llama a
 * la funció de_draw (que tiene la misma signatura que cpct_drawSprite). Esta
 * función añade el sprite a una cola. Después del VSYNC debe llamarse a la
 * función de_update que es la responsable de actualizar la pantalla.
 *
 */

#include <cpctelera.h>

/*
 * Inicializa el draw engine.
 *
 * Hay que llamarla una vez al principio del programa.
 *
 */
void de_init(void);


/*
 * Reinicia el draw engine.
 *
 * Vacia la cola.
 *
 */
void de_reset(void);


/*
 * Añade el sprite a la cola.
 *
 * La cola de pintado tiene capacidad para 16 sprites. Una vez llena las
 * llamadas a de_draw se ignoran.
 *
 */
void de_draw(u8 *sprite, u8 *vmem, u8 width, u8 height);

/*
 * Pinta los sprites de la cola.
 *
 * Una vez pintados los sprites se vacia la cola.
 *
 */
void de_update(void);

#endif /* _DRAWENG_H_ */
