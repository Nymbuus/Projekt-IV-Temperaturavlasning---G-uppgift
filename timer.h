#ifndef TIMER_H_
#define TIMER_H_

#include "header.h"

#define TIMER0_ELAPSE_TIME_MS 60000 // Timer 0 löper ut efter en minut.

#define TIMER0_COUNTER_MAX (uint32_t)(TIMER0_ELAPSE_TIME_MS / 0.128 + 0.5) // Vad timern ska räkna upp till

#endif /* TIMER_H_ */