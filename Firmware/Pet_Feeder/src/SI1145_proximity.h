/*
 * SI1145_proximity.h
 *
 *  Created on: 02-Mar-2022
 *      Author: saikr
 */

#ifndef SRC_SI1145_PROXIMITY_H_
#define SRC_SI1145_PROXIMITY_H_
#include <stdbool.h>
#include "src/i2c.h"
#include "scheduler.h"

typedef enum {
  STATE0_BEGIN_INIT,
  STATE1_SET_PS1_LED_CURRENT,
  STATE2_SET_PARAM_WR,
  STATE3_COMMAND_ENABLE_PS,
  STATE4_COMPLETE_INIT
}Proximity_State_t;

bool SI1145_proximity_init_SM(sl_bt_msg_t *);
#endif /* SRC_SI1145_PROXIMITY_H_ */
