/*
 * hm10_ble.c
 *
 *  Created on: 15 lug 2026
 *      Author: elyca
 */
#include "hm10_ble.h"

extern UART_HandleTypeDef huart1;

volatile uint8_t recieved = 0; //1 if i have recieved

void bt_transmit_int(int value){

	char buffer_tx[50];
	int len;

	if (value == MISSVALUE){
		len = snprintf(buffer_tx, sizeof(buffer_tx), "Reaction time: MISS\r\n");
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer_tx, len, 100);
	}
	else{
		len = snprintf(buffer_tx, sizeof(buffer_tx), "Reaction time: %i\r\n", value);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer_tx, len, 100);
	}


}

int bt_recieve_int(void){
	uint8_t rx_byte;

	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
	return (int)rx_byte;

}
