#ifndef HEADER_H_
#define HEADER_H_

// Alla inkluderingsdirektiv samlade på ett ställe.
#include "misc.h"
#include "tmp36.h"
#include "timer.h"
#include "serial.h"
#include "adc.h"

// definerar vilken pin knappen ska ligga på och när den är nedtryckt.
#define BUTTON1 5
#define BUTTON1_IS_PRESSED (PINB & (1 << BUTTON1))

// Definerar en temperatur struct och skapar en pekare till den.
extern struct tmp36 t1;
extern struct tmp36* t1ptr;

// Deklarerar funktioner.
void setup(void);
void button_init();

#endif /* HEADER_H_ */