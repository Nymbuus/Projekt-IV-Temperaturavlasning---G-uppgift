/********************************************************************************
* interrupts.c: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.h"

// Definerar variabeln counter0 med startv�rde 0.
static volatile uint32_t counter0 = 0;

/********************************************************************************
* ISR(PCINT0_vect): Trycks v�ran knapp ned s� skrivs tempertemperaturen i
*                   omgivningen ut och r�knaren nollst�lls inf�r n�sta 
*                   uppr�kning.
********************************************************************************/
ISR(PCINT0_vect)
{
	if (BUTTON1_IS_PRESSED)
	{
		tmp36_print_temperature(t1ptr);
		counter0 = 0;
	}
}

/********************************************************************************
* ISR (TIMER0_OVF_vect): Avbrottsrutin f�r Timer 0 i Normal Mode, d�r avbrott
*                        sker var 0.128:e ms vid overflow av r�knaren TCNT0.
*                        Varje exekverat avbrott r�knas via variabeln counter0.
*                        Efter en minut skrivs temperaturen i omgivningen ut och
*                        r�knaren nollst�lls inf�r n�sta uppr�kning.
********************************************************************************/
ISR (TIMER0_OVF_vect)
{
	if (++counter0 >= TIMER0_COUNTER_MAX)
	{
		tmp36_print_temperature(t1ptr);
		counter0 = 0;
	}
	return;
}