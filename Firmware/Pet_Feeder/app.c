/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Date:        08-07-2021
 * Author:      Rishab
 * Description: This code was created by the Silicon Labs application wizard
 *              and started as "Bluetooth - SoC Empty".
 *              It is to be used only for ECEN 5823 "IoT Embedded Firmware".
 *              The MSLA referenced above is in effect.
 *
 ******************************************************************************/


#include "app.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#define adcFreq   16000000

volatile uint32_t sample;
volatile float millivolts;

/*****************************************************************************
 * Application Power Manager callbacks
 *****************************************************************************/
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)



bool app_is_ok_to_sleep(void)
{

    return APP_IS_OK_TO_SLEEP;

} // app_is_ok_to_sleep()



sl_power_manager_on_isr_exit_t app_sleep_on_isr_exit(void)
{

    return APP_SLEEP_ON_ISR_EXIT;

} // app_sleep_on_isr_exit()



#endif // defined(SL_CATALOG_POWER_MANAGER_PRESENT)




/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{

    gpioInit(); // DOS, added this back in.

    // DOS
#if (LOWEST_ENERGY_LEVEL == EM1 || LOWEST_ENERGY_LEVEL == EM2)
            sl_power_manager_add_em_requirement(LOWEST_ENERGY_LEVEL);
#endif


    Osc_InitLETIMER0();

    LETIMER0_Init();

    LETIMER0_Start();


    gpioInit();
    PB0_Init();
    PB1_Init();
    I2C0_Init();
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

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
    // Put your application code here.
    // This is called repeatedly from the main while(1) loop
    // Notice: This function is not passed or has access to Bluetooth stack events.
    //         We will create/use a scheme that is far more energy efficient in
    //         later assignments.



    pet_feeder_event_t event = Scheduler_GetNextEvent();
    if (event == ev_PB_PRESSED) {
        LOG_INFO("PB pressed!\r\n");
        NVIC_ClearPendingIRQ(ADC0_IRQn);
        NVIC_EnableIRQ(ADC0_IRQn);
        ADC_Start(ADC0, adcStartSingle);
        while (!ADC_GetFlag());
        ADC_ClearFlag();
        sample = ADC_DataSingleGet(ADC0);

        // Calculate input voltage in mV
        millivolts = (sample * 3300.0) / 4096;
        if (millivolts > 1000) {
            gpioLed0SetOn();
        }
        else {
            gpioLed0SetOff();
        }
        LOG_INFO("%d\r\n", (int)millivolts);
    }


    //TemperatureStateMachine(event);
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *
 * The code here will process events from the Bluetooth stack. This is the only
 * opportunity we will get to act on an event.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{

    // Just a trick to hide a compiler warning about unused input parameter evt.
    // We will add real functionality here later.
    if (evt->header) {
            printf(".\n");
    }
} // sl_bt_on_event()

