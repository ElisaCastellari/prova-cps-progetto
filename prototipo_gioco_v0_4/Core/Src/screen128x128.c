/*
 * screen128x128.c
 *
 *  Created on: 7 lug 2026
 *      Author: elyca
 */
char buffer_schermo[32];


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
	ssd1306_SetCursor(5, 5);
	ssd1306_WriteString("FINAL RESULTS:",Font_7x10, White);
	printf("\n\n\nScore su Seriale: %i\r\n\n\n", score);
	int nuovoScore = (int)score;
	snprintf(buffer_schermo, sizeof(buffer_schermo), "Score: %ld", nuovoScore);
	ssd1306_SetCursor(5, 20);
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
    ssd1306_UpdateScreen();
}

void finalScore_screen_v2(void){
    // 1. Diamo una bella ripulita allo schermo (niente numeri vecchi in memoria!)
    ssd1306_Fill(Black);

    ssd1306_SetCursor(5, 5);
    ssd1306_WriteString("FINAL RESULTS:", Font_7x10, White);

    // --- TEST 1: NUMERO FINTO ---
    // Se questo non funziona, è rotta la funzione sprintf
    char str_finta[32];
    sprintf(str_finta, "Finto: 999");
    ssd1306_SetCursor(5, 20);
    ssd1306_WriteString(str_finta, Font_7x10, White);

    // --- TEST 2: LA TUA VARIABILE ---
    // Se questo stampa 0, il problema è il valore della variabile in QUESTO esatto millisecondo
    char str_vera[32];
    sprintf(str_vera, "Vero: %d", (int)score);
    ssd1306_SetCursor(5, 35);
    ssd1306_WriteString(str_vera, Font_7x10, White);

    // Mandiamo tutto allo schermo in un colpo solo
    ssd1306_UpdateScreen();
}


