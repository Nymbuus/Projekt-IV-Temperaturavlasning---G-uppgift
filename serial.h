/********************************************************************************
* serial.h: Innehåller funktionalitet för seriell överföring av text, heltal
*           och flyttal till en seriell terminal via USART.
********************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

/* Klockfrekvens (behövs för att sätta baud rate vid seriell överföring): */
#define F_CPU 16000000UL /* 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h>  /* Innehåller information om I/O-register såsom UCRS0B, DDRB med mera. */
#include <stdio.h>   /* Innehåller funktionen sprintf (typomvandlar tal till text). */
#include <stdbool.h> /* Innehåller datatypen bool. */

/********************************************************************************
* serial_init: Aktiverar seriell överföring för transmission av data med
*              angiven baud rate (överhastighet) i kbps (kilobits per sekund).
*              Vi ställer in asynkron överföring med åtta bitar i taget utan
*              stoppbit. Om seriell överföring redan har aktiverats så sker
*              ingen ny initiering.
*
*              - baud_rate_kbps: Baud rate i kilobits per sekund. 
********************************************************************************/
void serial_init(const uint32_t baud_rate_kbps);

/********************************************************************************
* serial_print_string: Skriver ut angivet textstycke till en seriell terminal.
*
*                      - s: Pekare till textstycken som ska skrivas ut.
********************************************************************************/
void serial_print_string(const char* s);

/********************************************************************************
* serial_print_integer: Skriver ut ett signerat heltal till en seriell terminal.
*
*                       - num: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_integer(const int32_t num);

/********************************************************************************
* serial_print_unsigned: Skriver ut ett osignerat heltal till en seriell terminal.
*
*                        - num: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_unsigned(const uint32_t num);

/********************************************************************************
* serial_print_double: Skriver ut ett flyttal med upp till två decimaler till
*                      en seriell terminal.
*
*                      - num: Flyttalet som ska skrivas ut.
********************************************************************************/
void serial_print_double(const double num);

/********************************************************************************
* serial_print_char: Skriver ut ett tecken till en seriell terminal. Innan 
*                    angivet tecken skrivs ut sker väntan på att eventuellt
*                    föregående tecken skickas.  
*
*                    - c: Tecknet som ska skrivas ut.
********************************************************************************/
void serial_print_char(const char c);

/********************************************************************************
* serial_print_new_line: Genererar en ny rad i en seriell terminal med nyrads-
*                        tecknet \n. Ett vagnreturstecken \r skrivs också ut för 
*                        att nästa utskrift ska hamna längst till vänster på 
*                        nästa rad.
********************************************************************************/
void serial_print_new_line(void);

#endif /* SERIAL_H_ */