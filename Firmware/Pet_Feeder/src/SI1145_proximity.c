/*
 * SI1145_proximity.c
 *
 *  Created on: 02-Mar-2022
 *      Author: saikr
 */

#include "SI1145_proximity.h"

uint8_t prox_wr_buff[]={0x00};
uint8_t wr1_buff_len=1;
uint8_t wr1_command_clear[]={0x18,0x00};
uint8_t wr1_param_wr1[]={0x17,0x01};
uint8_t wr1_command_proximity_en[]={0x18,0xA1};
uint8_t wr1_command_proximity_meas[]={0x18,0x05};
uint8_t wr1_proximity_led_current[]={0x0f,0x03};
uint8_t wr1_rd11_proximity_value[]={0x26};
uint8_t wr1_rd11_response[]={0x20};
uint8_t rd11_buff[]={};
uint8_t rd11_buff_len=1;

uint8_t device_id_buffer[1];

bool SI1145_proximity_init_SM(sl_bt_msg_t *evt)
{
  bool init_status = false;

  static Proximity_State_t next_init_state=STATE0_BEGIN_INIT;
  switch(next_init_state)
  {
    case STATE0_BEGIN_INIT:
      I2C0_WriteRead(0x60,prox_wr_buff,sizeof(prox_wr_buff),rd11_buff,rd11_buff_len);
      next_init_state= STATE1_SET_PS1_LED_CURRENT;
      break;

    case STATE1_SET_PS1_LED_CURRENT:
      if(evt->data.evt_system_external_signal.extsignals == ev_I2C_Tx_complete)
      {
          //assert(device_id_buffer[0] == SI1145_PART_ID);
          //LOG_INFO("%x\n",device_id_buffer[0]);
          //SI1145_set_LED1();
          //LOG_INFO("Writing param WR register %d\n\r",I2C0_WriteWrite(0x60,wr1_param_wr1,sizeof(wr1_param_wr1)));
          I2C0_WriteWrite(0x60,wr1_param_wr1,sizeof(wr1_param_wr1));
          next_init_state = STATE2_SET_PARAM_WR;
      }
      break;

    case STATE2_SET_PARAM_WR:
      if(evt->data.evt_system_external_signal.extsignals == ev_I2C_Tx_complete)
      {
          //SI1145_set_param();
//          LOG_INFO("Writing COMMAND to enable proximity %d\n\r",I2C0_WriteWrite(0x60,wr1_proximity_led_current,sizeof(wr1_proximity_led_current)));
          I2C0_WriteWrite(0x60,wr1_proximity_led_current,sizeof(wr1_proximity_led_current));
          next_init_state = STATE3_COMMAND_ENABLE_PS;
      }
      break;
    case STATE3_COMMAND_ENABLE_PS:
      if(evt->data.evt_system_external_signal.extsignals == ev_I2C_Tx_complete)
      {
//          SI1145_command_ps1_enable();
//          LOG_INFO("Writing COMMAND to enable proximity %d\n\r",I2C0_WriteWrite(0x60,wr1_command_proximity_en,sizeof(wr1_command_proximity_en)));
          I2C0_WriteWrite(0x60,wr1_command_proximity_en,sizeof(wr1_command_proximity_en));
          next_init_state = STATE4_COMPLETE_INIT;
      }
      break;
    case STATE4_COMPLETE_INIT:
      if(evt->data.evt_system_external_signal.extsignals == ev_I2C_Tx_complete)
         init_status = true;
      break;
    default:
      break;
  }
  return init_status;
}
