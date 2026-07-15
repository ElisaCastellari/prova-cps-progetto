/*
 * hm10_ble.h
 *
 *  Created on: 15 lug 2026
 *      Author: elyca
 */

#ifndef INC_HM10_BLE_H_
#define INC_HM10_BLE_H_

#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"
#include "constants.h"

void bt_transmit_int(int value);
int bt_recieve_int(void);

#endif /* INC_HM10_BLE_H_ */
