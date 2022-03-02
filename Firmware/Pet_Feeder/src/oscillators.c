/*
 * oscillators.c - Oscillator functions
 *
 *  Created on: Sep 5, 2021
 *      Author: vishnu
 */

#include "oscillators.h"

void Osc_InitLETIMER0(void) {
    if (LOWEST_ENERGY_LEVEL == EM3) {
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
        CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_1);
        CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
        CMU_ClockEnable(cmuClock_LFA, true);
        CMU_ClockEnable(cmuClock_LETIMER0, true);
    }
    else {
        CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
        CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
        CMU_ClockEnable(cmuClock_LFA, true);
        CMU_ClockEnable(cmuClock_LETIMER0, true);
    }
}
