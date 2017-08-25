#ifndef _KBD_H_
#define _KBD_H_

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

#endif /* _KBD_H_ */
