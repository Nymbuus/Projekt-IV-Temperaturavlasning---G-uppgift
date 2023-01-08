/********************************************************************************
* serial.h: Inneh�ller funktionalitet f�r seriell �verf�ring av text, heltal
*           och flyttal till en seriell terminal via USART.
********************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

/* Klockfrekvens (beh�vs f�r att s�tta baud rate vid seriell �verf�ring): */
#define F_CPU 16000000UL /* 16 MHz. */

/* Inkluderingsdirektiv: */
#include <avr/io.h>  /* Inneh�ller information om I/O-register s�som UCRS0B, DDRB med mera. */
#include <stdio.h>   /* Inneh�ller funktionen sprintf (typomvandlar tal till text). */
#include <stdbool.h> /* Inneh�ller datatypen bool. */

/********************************************************************************
* serial_init: Aktiverar seriell �verf�ring f�r transmission av data med
*              angiven baud rate (�verhastighet) i kbps (kilobits per sekund).
*              Vi st�ller in asynkron �verf�ring med �tta bitar i taget utan
*              stoppbit. Om seriell �verf�ring redan har aktiverats s� sker
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
* serial_print_double: Skriver ut ett flyttal med upp till tv� decimaler till
*                      en seriell terminal.
*
*                      - num: Flyttalet som ska skrivas ut.
********************************************************************************/
void serial_print_double(const double num);

/********************************************************************************
* serial_print_char: Skriver ut ett tecken till en seriell terminal. Innan 
*                    angivet tecken skrivs ut sker v�ntan p� att eventuellt
*                    f�reg�ende tecken skickas.  
*
*                    - c: Tecknet som ska skrivas ut.
********************************************************************************/
void serial_print_char(const char c);

/********************************************************************************
* serial_print_new_line: Genererar en ny rad i en seriell terminal med nyrads-
*                        tecknet \n. Ett vagnreturstecken \r skrivs ocks� ut f�r 
*                        att n�sta utskrift ska hamna l�ngst till v�nster p� 
*                        n�sta rad.
********************************************************************************/
void serial_print_new_line(void);

#endif /* SERIAL_H_ */