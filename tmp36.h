 /********************************************************************************
* tmp36.h: Inneh�ller drivrutiner f�r avl�sning samt utskrift av rumstemperatur
*          med temperatursensor TMP36.
*
*          Rumstemperaturen T ber�knas med f�ljande formel:
*
*          T = 100 * Uin - 50,
*
*          d�r T utg�r temperaturen i Celcius och Uin utg�r insp�nningen
*          fr�n temperatursensorn (0 - 5 V).
*
*          Den analoga insp�nningen Uin kan ber�knas via f�ljande formel:
*
*          Uin = ADC_result / ADC_MAX * Vcc,
*
*          d�r ADC_result utg�r avl�st resultat fr�n AD-omvandlaren (0 - 1023),
*          ADC_MAX utg�r h�gsta m�jliga resultat fr�n AD-omvandlaren (1023)
*          och Vcc utg�r AD-omvandlarens matningssp�nning (5.0 V).
********************************************************************************/
#ifndef TMP36_H_
#define TMP36_H_

/* Inkluderingsdirektiv: */
#include "adc.h"
#include "serial.h"

/* Makrodefinitioner: */
#define A0 PORTC0 /* Analog pin A0 (PORTC0). */
#define A1 PORTC1 /* Analog pin A1 (PORTC1). */
#define A2 PORTC2 /* Analog pin A2 (PORTC2). */
#define A3 PORTC3 /* Analog pin A3 (PORTC3). */
#define A4 PORTC4 /* Analog pin A4 (PORTC4). */
#define A5 PORTC5 /* Analog pin A5 (PORTC5). */

/********************************************************************************
* tmp36: Strukt f�r implementering av temperatursensor TMP36 i samband med
*        associerade drivrutiner.
********************************************************************************/
struct tmp36
{
   uint8_t pin; /* Analog pin A0 - A5 som temperatursensorn �r ansluten till. */
};

/********************************************************************************
* tmp36_init: Initierar temperaturm�tning med temperatursensor TMP36 genom att
*             initiera AD-omvandlarens analoga kretsar samt initiera seriell
*             �verf�ring med en baud rate (�verf�ringshastighet) p� 9600 kbps.
********************************************************************************/
static inline void tmp36_init(struct tmp36* self,
                              const uint8_t pin)
{
   self->pin = pin;
   (void)adc_read(A0);
   serial_init(9600);
   return;
}

/********************************************************************************
* tmp36_get_temperature: Returnerar aktuell rumstemperatur uppm�tt med
*                        temperatursensor TMP36 som ett flyttal.
********************************************************************************/
static inline double tmp36_get_temperature(const struct tmp36* self)
{
   const double voltage = adc_read(self->pin) / ADC_MAX * VCC;
   return 100 * voltage - 50;
}

/********************************************************************************
* tmp36_print_temperature: Skriver ut aktuell rumstemperatur uppm�tt med
*                          temperatursensor TMP36 via ansluten seriell terminal.
********************************************************************************/
static inline void tmp36_print_temperature(const struct tmp36* self)
{
   serial_print_string("Temperature: ");
   serial_print_double(tmp36_get_temperature(self));
   serial_print_string(" degrees Celcius\n");
   return;
}

#endif /* TMP36_H_ */