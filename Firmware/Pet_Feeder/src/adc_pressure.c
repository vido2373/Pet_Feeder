/*
 * adc_pressure.c
 *
 *  Created on: 02-Mar-2022
 *      Author: saikr
 */

#include "adc_pressure.h"

#define adcFreq   16000000

void adcInit()
{
  // Enable ADC0 clock
      CMU_ClockEnable(cmuClock_ADC0, true);

      // Declare init structs
      ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
      ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

      // Modify init structs and initialize
      init.prescale = ADC_PrescaleCalc(adcFreq, 0); // Init to max ADC clock for Series 1
      init.timebase = ADC_TimebaseCalc(0);

      initSingle.diff       = false;        // single ended
      initSingle.reference  = adcRefVDD;    // internal 2.5V reference
      initSingle.resolution = adcRes12Bit;  // 12-bit resolution
      initSingle.acqTime    = adcAcqTime4;  // set acquisition time to meet minimum requirements

      // Select ADC input. See README for corresponding EXP header pin.
      initSingle.posSel = adcPosSelAPORT2XCH9;

      ADC_Init(ADC0, &init);
      ADC_InitSingle(ADC0, &initSingle);

      // Enable ADC Single Conversion Complete interrupt
      ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

      // Enable ADC interrupts
      NVIC_ClearPendingIRQ(ADC0_IRQn);
      NVIC_EnableIRQ(ADC0_IRQn);
}
