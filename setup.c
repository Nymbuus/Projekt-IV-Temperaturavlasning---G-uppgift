/********************************************************************************
* setup.c: Det första som händer i programmet där vi initierar allt nödvändigt.
********************************************************************************/
#include "header.h"

// Deklarerar staisk funktion.
static inline void timer_init();

// Deklararer och definerar objektet t1 och pekare till dens adress.
struct tmp36 t1;
struct tmp36* t1ptr = &t1;

/********************************************************************************
* setup: Innehåller initieringen för knappen, tempsensorn och timern.
********************************************************************************/
void setup()
{
	asm("SEI");
	button_init();
	tmp36_init(&t1, A2);
	timer_init();
}

/********************************************************************************
* button_init: Initierar knappen och aktiverar avbrott på den.
********************************************************************************/
void button_init()
{
	PORTB = (1 << BUTTON1);
	
	PCICR = (1 << PCIE0);
	PCMSK0 = (1 << BUTTON1);
}

/********************************************************************************
* init_timers: Aktiverar timerkretsen Timer0 till att generera avbrott
*              var 0.128 ms.
********************************************************************************/
static inline void timer_init()
{
	TCCR0B = (1 << CS01);
	TIMSK0 = (1 << TOIE0);
}