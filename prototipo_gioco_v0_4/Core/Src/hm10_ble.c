/*
 * hm10_ble.c
 *
 *  Created on: 15 lug 2026
 *      Author: elyca
 */
#include "hm10_ble.h"


extern UART_HandleTypeDef huart1;
extern char buffer_schermo[128];

volatile uint8_t recieved = 0; //1 if i have recieved
extern uint8_t rx_string[50];

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
	// ricevuto è il numero int
	// rx_string è ciò che si riceve che parte come string quindi ascii e non solo un char penso
	// effettivamente se usiamo uint8_t rimane un solo byte ma quale di preciso?
	// noi vogliamo solo il primo byte
	uint8_t ricevuto =0;

	for (int i = 0; i < 50; i++){
	sprintf(buffer_schermo, "sono %u, %i \r\n", rx_string[i], i);
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer_schermo, strlen(buffer_schermo), 100);
	osDelay(10);
	}
	//uint8_t rx_byte;
	//return (int)rx_byte;
	if ((int)rx_string[0] >= (int)'1' && (int)rx_string[0] <= (int)'9'){
		ricevuto = ((int) rx_string[0])-48;
		sprintf(buffer_schermo, "sono entrato \r\n");
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer_schermo, strlen(buffer_schermo), 100);
	}
	else {ricevuto = 0;
	sprintf(buffer_schermo, "sono fuori \r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer_schermo, strlen(buffer_schermo), 100);
	}
	return ricevuto;
}
