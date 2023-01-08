/********************************************************************************
* interrupts.c: Innehåller avbrottsrutiner.
********************************************************************************/
#include "header.h"

// Definerar variabeln counter0 med startvärde 0.
static volatile uint32_t counter0 = 0;

/********************************************************************************
* ISR(PCINT0_vect): Trycks våran knapp ned så skrivs tempertemperaturen i
*                   omgivningen ut och räknaren nollställs inför nästa 
*                   uppräkning.
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
* ISR (TIMER0_OVF_vect): Avbrottsrutin för Timer 0 i Normal Mode, där avbrott
*                        sker var 0.128:e ms vid overflow av räknaren TCNT0.
*                        Varje exekverat avbrott räknas via variabeln counter0.
*                        Efter en minut skrivs temperaturen i omgivningen ut och
*                        räknaren nollställs inför nästa uppräkning.
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