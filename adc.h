/********************************************************************************
* adc.h: Inneh�ller drivrutiner f�r AD-omvandling samt PWM-generering.
********************************************************************************/
#ifndef ADC_H_
#define ADC_H_

/* Inkluderingsdirektiv: */
#include <avr/io.h>

/* Makrodefinitioner: */
#define ADC_MAX 1023.0 /* H�gsta m�jliga resultat vid AD-omvandling. */
#define VCC 5.0        /* 5.0 V matningssp�nning. */

/********************************************************************************
* adc_read: L�ser av angiven analog pin och returnerar motsvarande digitala
*           v�rde mellan 0 - 1023.
*
*           - pin: Analog pin A0 - A5 som ska l�sas av.
********************************************************************************/
uint16_t adc_read(const uint8_t pin);

/********************************************************************************
* adc_get_pwm_values: L�ser av angiven analog pin och ber�knar on- och off-tid
*                     utefter angiven periodtid. Ber�knar on- och off-tid lagras
*                     p� angivna adresser.
*
*                     - pin          : Analog pin A0 - A5 som ska l�sas av.
*                     - pwm_period_ms: Periodtid f�r PWM-generering i ms.
*                     - pwm_on_ms    : Lagringsplats f�r ber�knad on-tid i ms.
*                     - pwm_off_ms   : Lagringsplats f�r ber�knad on-tid i ms.
********************************************************************************/
void adc_get_pwm_values(const uint8_t pin,
                        const uint8_t pwm_period_ms,
                        uint8_t* pwm_on_ms,
                        uint8_t* pwm_off_ms);

#endif /* ADC_H_ */