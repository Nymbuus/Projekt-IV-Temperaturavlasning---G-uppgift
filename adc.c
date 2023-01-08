/********************************************************************************
* adc.c: Inneh�ller drivrutiner f�r AD-omvandling samt PWM-generering.
********************************************************************************/
#include "header.h"

/********************************************************************************
* adc_read: L�ser av angiven analog pin och returnerar motsvarande digitala
*           v�rde mellan 0 - 1023.
*
*           1. Vi v�ljer att anv�nda intern matningssp�nning AVcc (5 V)
*              f�r att mata AD-omvandlaren genom att ettst�lla biten REFS0 i
*              register ADMUX (ADC Multiplexer Select Register).
*
*           2. Vi v�ljer vilken analog kanal som ska kopplas till
*              AD-omvandlaren genom att skriva till selektorbitar MUX[3:0] i
*              samma register. Detta v�rde ska av pin-numret rakt av utan
*              bitvis skiftning. Exempelvis, f�r att l�sa av analog pin A2
*              s� ska MUX[3:0] s�ttas till 0010, vilket �stadkommes genom att
*              anv�nda bitvis or, d�r | 2 medf�r bitvis addition med 0010.
*
*           3. Vi aktiverar AD-omvandlaren och startar omvandling direkt genom
*              att ettst�lla bitar ADEN (ADC Enable) samt ADSC (ADC Start
*              Conversion) i kontroll- och statusregister ADCSRA (ADC Control
*              And Status Register A).
*
*           4. Vi s�tter klockfrekvensen f�r AD-omvandlaren till 125 kHz,
*              vilket �r inom den rekommenderade zonen f�r maximal uppl�sning
*              (50 kHz - 200 kHz) genom att ettst�lla prescaler-bitar ADPS[2:0]
*              (ADC Prescaler Select [2:0]) i samma register, vilket ger en
*              divisionsfaktor p� 128 av mikrodatorns klockfrekvens. D�rmed blir
*              AD-omvandlarens klockfrekvens lika med 16M / 128 = 125 KHz.
*
*           5. Vi v�ntar p� att AD-omvandling blir slutf�rd, vilket indikeras
*              genom att biten ADIF (ADC Interrupt Flag) i register ADCSRA
*              blir ettst�lld. S� l�nge ADIF �r noll v�ntar vi via en while-sats.
*
*           6. Vi �terst�ller ADIF inf�r n�sta AD-omvandling genom att ettst�lla
*              denna. N�sta klockcykel blir ADIF d� nollst�lld av h�rdvaran.
*
*           7. Resultatet ligger nu i det 16-bitars registret ADC (egentligen
*              tv� register ADCH och ADCL) och returneras direkt.
*
*           - pin: Analog pin A0 - A5 som ska l�sas av.
********************************************************************************/
uint16_t adc_read(const uint8_t pin)
{
   ADMUX = (1 << REFS0) | pin;
   ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   while ((ADCSRA & (1 << ADIF)) == 0);
   ADCSRA = (1 << ADIF);
   return ADC;
}

/********************************************************************************
* adc_get_pwm_values: L�ser av angiven analog pin och ber�knar on- och off-tid
*                     utefter angiven periodtid. Ber�knar on- och off-tid lagras
*                     p� angivna adresser.
*
*                     1. Ber�knar duty cycle som ett tal mellan 0 - 1 genom att
*                        dividera det avl�sta v�rdet fr�n angiven analog pin
*                        (returneras fr�n adc_read) genom maxv�rdet 1023.
*
*                     2. Vi ber�knar on-tiden genom att multiplicera periodtiden
*                        med duty cycle, exempelvis 10 ms * 0.6 = 6 ms.
*                        Resultatet avrundas till n�rmaste heltal.
*
*                     3. Vi ber�knar off-tiden som resterande periodtid, dvs.
*                        periodtiden - on-tiden.
*
*                     - pin          : Analog pin A0 - A5 som ska l�sas av.
*                     - pwm_period_ms: Periodtid f�r PWM-generering i ms.
*                     - pwm_on_ms    : Lagringsplats f�r ber�knad on-tid i ms.
*                     - pwm_off_ms   : Lagringsplats f�r ber�knad on-tid i ms.
********************************************************************************/
void adc_get_pwm_values(const uint8_t pin,
                        const uint8_t pwm_period_ms,
                        uint8_t* pwm_on_ms,
                        uint8_t* pwm_off_ms)
{
   const double duty_cycle = adc_read(pin) / 1023.0;
   *pwm_on_ms = (uint8_t)(pwm_period_ms * duty_cycle + 0.5);
   *pwm_off_ms = pwm_period_ms - *pwm_on_ms;
   return;
}