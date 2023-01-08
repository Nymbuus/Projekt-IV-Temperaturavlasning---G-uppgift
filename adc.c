/********************************************************************************
* adc.c: Innehåller drivrutiner för AD-omvandling samt PWM-generering.
********************************************************************************/
#include "header.h"

/********************************************************************************
* adc_read: Läser av angiven analog pin och returnerar motsvarande digitala
*           värde mellan 0 - 1023.
*
*           1. Vi väljer att använda intern matningsspänning AVcc (5 V)
*              för att mata AD-omvandlaren genom att ettställa biten REFS0 i
*              register ADMUX (ADC Multiplexer Select Register).
*
*           2. Vi väljer vilken analog kanal som ska kopplas till
*              AD-omvandlaren genom att skriva till selektorbitar MUX[3:0] i
*              samma register. Detta värde ska av pin-numret rakt av utan
*              bitvis skiftning. Exempelvis, för att läsa av analog pin A2
*              så ska MUX[3:0] sättas till 0010, vilket åstadkommes genom att
*              använda bitvis or, där | 2 medför bitvis addition med 0010.
*
*           3. Vi aktiverar AD-omvandlaren och startar omvandling direkt genom
*              att ettställa bitar ADEN (ADC Enable) samt ADSC (ADC Start
*              Conversion) i kontroll- och statusregister ADCSRA (ADC Control
*              And Status Register A).
*
*           4. Vi sätter klockfrekvensen för AD-omvandlaren till 125 kHz,
*              vilket är inom den rekommenderade zonen för maximal upplösning
*              (50 kHz - 200 kHz) genom att ettställa prescaler-bitar ADPS[2:0]
*              (ADC Prescaler Select [2:0]) i samma register, vilket ger en
*              divisionsfaktor på 128 av mikrodatorns klockfrekvens. Därmed blir
*              AD-omvandlarens klockfrekvens lika med 16M / 128 = 125 KHz.
*
*           5. Vi väntar på att AD-omvandling blir slutförd, vilket indikeras
*              genom att biten ADIF (ADC Interrupt Flag) i register ADCSRA
*              blir ettställd. Så länge ADIF är noll väntar vi via en while-sats.
*
*           6. Vi återställer ADIF inför nästa AD-omvandling genom att ettställa
*              denna. Nästa klockcykel blir ADIF då nollställd av hårdvaran.
*
*           7. Resultatet ligger nu i det 16-bitars registret ADC (egentligen
*              två register ADCH och ADCL) och returneras direkt.
*
*           - pin: Analog pin A0 - A5 som ska läsas av.
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
* adc_get_pwm_values: Läser av angiven analog pin och beräknar on- och off-tid
*                     utefter angiven periodtid. Beräknar on- och off-tid lagras
*                     på angivna adresser.
*
*                     1. Beräknar duty cycle som ett tal mellan 0 - 1 genom att
*                        dividera det avlästa värdet från angiven analog pin
*                        (returneras från adc_read) genom maxvärdet 1023.
*
*                     2. Vi beräknar on-tiden genom att multiplicera periodtiden
*                        med duty cycle, exempelvis 10 ms * 0.6 = 6 ms.
*                        Resultatet avrundas till närmaste heltal.
*
*                     3. Vi beräknar off-tiden som resterande periodtid, dvs.
*                        periodtiden - on-tiden.
*
*                     - pin          : Analog pin A0 - A5 som ska läsas av.
*                     - pwm_period_ms: Periodtid för PWM-generering i ms.
*                     - pwm_on_ms    : Lagringsplats för beräknad on-tid i ms.
*                     - pwm_off_ms   : Lagringsplats för beräknad on-tid i ms.
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