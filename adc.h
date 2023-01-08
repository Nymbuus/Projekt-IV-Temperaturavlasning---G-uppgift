/********************************************************************************
* adc.h: Innehåller drivrutiner för AD-omvandling samt PWM-generering.
********************************************************************************/
#ifndef ADC_H_
#define ADC_H_

/* Inkluderingsdirektiv: */
#include <avr/io.h>

/* Makrodefinitioner: */
#define ADC_MAX 1023.0 /* Högsta möjliga resultat vid AD-omvandling. */
#define VCC 5.0        /* 5.0 V matningsspänning. */

/********************************************************************************
* adc_read: Läser av angiven analog pin och returnerar motsvarande digitala
*           värde mellan 0 - 1023.
*
*           - pin: Analog pin A0 - A5 som ska läsas av.
********************************************************************************/
uint16_t adc_read(const uint8_t pin);

/********************************************************************************
* adc_get_pwm_values: Läser av angiven analog pin och beräknar on- och off-tid
*                     utefter angiven periodtid. Beräknar on- och off-tid lagras
*                     på angivna adresser.
*
*                     - pin          : Analog pin A0 - A5 som ska läsas av.
*                     - pwm_period_ms: Periodtid för PWM-generering i ms.
*                     - pwm_on_ms    : Lagringsplats för beräknad on-tid i ms.
*                     - pwm_off_ms   : Lagringsplats för beräknad on-tid i ms.
********************************************************************************/
void adc_get_pwm_values(const uint8_t pin,
                        const uint8_t pwm_period_ms,
                        uint8_t* pwm_on_ms,
                        uint8_t* pwm_off_ms);

#endif /* ADC_H_ */