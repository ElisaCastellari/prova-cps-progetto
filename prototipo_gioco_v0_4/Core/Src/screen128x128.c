/*
 * screen128x128.c
 *
 *  Created on: 7 lug 2026
 *      Author: elyca
 */
char buffer_schermo[128];


extern int combo; //tiene conto la combo
extern volatile int score; //tiene conto dello score
extern int perfect;
extern int great;
extern int good;
extern int bad;
extern int miss;
extern int bestCombo; //to keep track of the best combo
extern float avgResponseTime; //to make average response time later
extern int hitNotes;  // to sum all hit notes and make average

#include "screen128x128.h"

void finalScore_screen(void){ //per printare i risultati completi a fine game

	ssd1306_Fill(Black);       // Svuota la memoria cancellando tutti i pixel
	ssd1306_DrawRectangle(0, 0, 127, 127, White);
	ssd1306_UpdateScreen();

	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("FINAL RESULTS:",Font_7x10, White);
	printf("\n\n\nScore su Seriale: %i\r\n\n\n", score);
	snprintf(buffer_schermo, sizeof(buffer_schermo), "Score: %i", score);
	ssd1306_SetCursor(5, 20);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "Best Combo: %i", bestCombo);
	ssd1306_SetCursor(5, 35);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "Average response time: %i", (int)(avgResponseTime / hitNotes)); //average response time, casted as int for semplicity
		if (bestCombo == SONGLENGHT){
			ssd1306_SetCursor(5, 50);
			snprintf(buffer_schermo, sizeof(buffer_schermo), "Full combo!");
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);
			if (perfect == SONGLENGHT){
				ssd1306_SetCursor(5, 65);
				snprintf (buffer_schermo, sizeof(buffer_schermo), "all perfect!");
				ssd1306_WriteString(buffer_schermo,Font_7x10, White);
			}
		}

		ssd1306_UpdateScreen();

	osDelay(5000);
		ssd1306_Fill(Black);       // Svuota la memoria cancellando tutti i pixel
		ssd1306_DrawRectangle(0, 0, 127, 127, White);
		ssd1306_UpdateScreen();    // Invia il comando allo schermo fisico per spegnerlo

		osDelay(200);


	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("RESULTS DETAILS:",Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "PERFECT: %i", perfect);
	ssd1306_SetCursor(5, 20);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "GREAT: %i", great);
	ssd1306_SetCursor(5, 35);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "GOOD: %i", good);
	ssd1306_SetCursor(5, 50);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "BAD: %i", bad);
	ssd1306_SetCursor(5, 65);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "MISS: %i", miss);
	ssd1306_SetCursor(5, 80);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	/*printf("Score: %i\r\n", score);
	printf("Best Combo: %i\r\n", bestCombo);
	printf("perfect: %i\r\n", perfect);
	printf("great: %i\r\n", great);
	printf("good: %i\r\n", good);
	printf("bad: %i\r\n", bad);
	printf("miss: %i\r\n", miss);
	printf("Average response time: %i\r\n", (int)(avgResponseTime / hitNotes)); //average response time, casted as int for semplicity
	if (bestCombo == SONGLENGHT){
		printf("Full combo! \r\n");
		if (perfect == SONGLENGHT){
			printf ("all perfect!\r\n");
		}
	}
	printf("---------------------------------------------------\r\n");
	resetScore(); // i reset to be ready for a new game
	*/
    ssd1306_UpdateScreen();
}

void screen_init(void){
    // 1. Inizializzazione standard
    ssd1306_Init();

    // 2. PATCH SH1107: Inviamo comandi grezzi via I2C per fixare il Multiplexer
    // 0xA8 è il registro del Multiplexer, 0x7F significa "attiva 128 linee"
    uint8_t sh1107_patch[] = { 0x00, 0xA8, 0x7F };
    HAL_I2C_Master_Transmit(&hi2c1, (0x3C << 1), sh1107_patch, sizeof(sh1107_patch), 100);

    // 3. Disegniamo un rettangolo sui bordi per verificare la centratura
    ssd1306_Fill(Black);
    ssd1306_DrawRectangle(0, 0, 127, 127, White);

    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("RHYTHM GAME PROTOTYPE",Font_7x10, White);


    ssd1306_UpdateScreen();
}

