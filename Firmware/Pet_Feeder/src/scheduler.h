/*
 * scheduler.h
 *
 *  Created on: Feb 21, 2022
 *      Author: vishn
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#define EVENT_QUEUE_SIZE          (128)
#define EVENT_QUEUE_SIZE_MASK     (0x7F)

typedef enum {
    ev_PB_PRESSED,
    ev_ADC_COMPLETE,
    ev_NONE
} pet_feeder_event_t;


/*
 * Sets PB_PRESSED event
 *
 * @param None
 *
 * @return None
 */
void Scheduler_SetEvent_PB_PRESSED(void);


/*
 * Sets ADC_COMPLETE event
 *
 * @param None
 *
 * @return None
 */
void Scheduler_SetEvent_ADC_COMPLETE(void);


/*
 * Returns next event in queue
 *
 * @param None
 *
 * @return None
 */
pet_feeder_event_t Scheduler_GetNextEvent(void);



#endif /* SRC_SCHEDULER_H_ */
