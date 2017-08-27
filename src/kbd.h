#ifndef _KBD_H_
#define _KBD_H_

#include <cpctelera.h>

/*
 * Espera a que no haya ninguna tecla pulsada.
 */
void kbd_discard_keys(void);

/*
 * Espera a que se pulse una tecla.
 *
 * Si se pasa un valor distinto de cero en el paràmetro discard se llama a
 * kbd_discard_keys.
 */
void kbd_wait_any_key(unsigned char discard);


/*
 * Gestión del teclado.
 *
 * NOTE: Todo esto es preliminar.
 *
 * Durante el juego quiero poder activar/desactivar algunas cosas para
 * facilitar el desarrollo, por ejemplo, que muestre info de debug: una
 * primera pulsación de la tecla D muestra la info, una segunda pulsación la
 * oculta etc.
 *
 * Una primera idea seria leer el estado de la tecla D en cada iteración del
 * bucle bucle del juego, si está pulsada se alterna el valor de cierta
 * variable global. Esto no funcionará. Durante los pocos milisegundos en los
 * que mantenemos la tecla pulsada el bucle del juego ejecutará un número
 * indeterminado de iteraciones. En cada iteración se alternará el valor de la
 * variable (1 -> 0 -> 1 ...). El valor final de la variable, cuando soltemos
 * la tecla, dependerá de si el número de iteraciones ha sido par o impar. No
 * resulta muy fiable.
 *
 * Si el problema es que se lee el teclado con demasiada frecuencia podemos
 * pensar en utilizar un reloj para leerlo solo a determinados intervalos. La
 * cosa mejora pero no es perfecta. Puede darse la situación contraria, una
 * pulsación rápida no es detectada porqué es mas corta que el intervalo de
 * lectura. Se puede jugar con el intervalo pero la sensación general es que
 * tampoco es 100% fiable.
 *
 * La solución implementada en este módulo define la pulsación de una tecla
 * como la secuencia de eventos "pulsar tecla" seguido de "soltar tecla".
 *
 * - al principio del programa hay que llamar a kbd_init() para inicializar el
 *   estado interno del módulo.
 *
 * - en cada iteración del bucle de juego hay que llamar a kbd_update(...).
 *   Esta función monitoriza una serie de teclas comprobando sus cambios de
 *   estado.
 *
 * - finalmente debemos llamar a kbd_test_keyup(...) para comprobar si alguna
 *   de las teclas monitorizadas ha cambiado.
 *
 * El funcionamiento se puede modelar como un autómata con tres estados por
 * cada tecla monitorizada:
 *
 *                           DOWN  <--------  UP
 *                             |              ^
 *                             |              |
 *                             +---> KEYUP ---+
 *
 * - kbd_init assigna el estado UP (tecla no pulsada) a todas las teclas.
 *
 * - al llamar a kbd_update:
 *
 *   - si la tecla está pulsada (cpct_isKeyPressed devuelve true) pasa al
 *     estado DOWN.
 *
 *   - si no está pulsada, si el estado es DOWN pasa a KEYUP si no pasa a UP
 *
 * - kbd_test_keyup devuelve true si el estado es KEYUP, esto es, si se ha
 *   producido la transició de "tecla pulsada" a "tecla soltada". Hay que
 *   tener presente que en la siguiente iteración kbd_update cambiará el
 *   estado nuevamente a UP.
 *
 */
void kbd_init(void);
void kbd_update(cpct_keyID *keys);
u8 kbd_test_keyup(cpct_keyID key) __z88dk_fastcall;

#endif /* _KBD_H_ */
