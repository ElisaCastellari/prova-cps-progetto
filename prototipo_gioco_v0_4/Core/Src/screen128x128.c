/*
 * screen128x128.c
 *
 *  Created on: 7 lug 2026
 *      Author: elyca
 */

#include "screen128x128.h"

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
extern const GameNote_t* melody;
extern const Song_t* libreriaCanzoni[];
extern uint8_t songSelection;


////////////////////nuove messe da me//////////////////////////////

extern volatile uint8_t trasmissione_in_corso;

// Screenbuffer
uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// Screen object
SSD1306_t SSD1306;

void ssd1306_WriteData_DMA(uint8_t* buffer, size_t buff_size) {
	while(trasmissione_in_corso){
		//osDelay(1);
	}

	if(trasmissione_in_corso == 0){
		trasmissione_in_corso = 1;
		HAL_I2C_Mem_Write_DMA(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x40, 1, buffer, buff_size);
	}
}

void ssd1306_WriteCommand_DMA(uint8_t byte) {
	while(trasmissione_in_corso){
		//osDelay(1);
	}

	if(trasmissione_in_corso == 0){
		trasmissione_in_corso = 1;
		HAL_I2C_Mem_Write_DMA(&SSD1306_I2C_PORT, SSD1306_I2C_ADDR, 0x00, 1, &byte, 1);
	}
}

void ssd1306_UpdateScreen_DMA(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_WriteCommand_DMA(0xB0 + i); // Set the current RAM page address.
        ssd1306_WriteCommand_DMA(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd1306_WriteCommand_DMA(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd1306_WriteData_DMA(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}


char ssd1306_WriteChar_DMA(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
    uint32_t i, b, j;

    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;

    // Char width is not equal to font width for proportional font
    const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;
    // Check remaining space on current line
    if (SSD1306_WIDTH < (SSD1306.CurrentX + char_width) ||
        SSD1306_HEIGHT < (SSD1306.CurrentY + Font.height))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < char_width; j++) {
            if((b << j) & 0x8000)  {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            } else {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    SSD1306.CurrentX += char_width;

    // Return written char for validation
    return ch;
}

char ssd1306_WriteString_DMA(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
    while (*str) {
        if (ssd1306_WriteChar_DMA(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        str++;
    }

    // Everything ok
    return *str;
}

////////////////////////////////////////////////////////////////////




void finalScore_screen(void){ //per printare i risultati completi a fine game

	//ssd1306_Fill(Black);       // Svuota la memoria cancellando tutti i pixel
	//ssd1306_DrawRectangle(0, 0, 127, 127, White);
	//ssd1306_UpdateScreen();

	clean_screen();

	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*0) ); // prima riga
	ssd1306_WriteString("FINAL RESULTS:",Font_7x10, White);
	//printf("\n\n\nScore su Seriale: %i\r\n\n\n", score);
	snprintf(buffer_schermo, sizeof(buffer_schermo), "Score: %i", score);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga moltiplicato per 1 salta la prima riga siccome è la seconda riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "Best Combo: %i", bestCombo);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) );  // terza riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	//response time. era dimenticato prima
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*3) );  // quarta riga
	ssd1306_WriteString("Average response",Font_7x10, White);

	if(hitNotes != 0){ //per evitare divisioni per zero
		snprintf(buffer_schermo, sizeof(buffer_schermo), "time: %i ms", (int)(avgResponseTime / hitNotes)); //average response time, casted as int for semplicity
	} else {
		snprintf(buffer_schermo, sizeof(buffer_schermo),"time : ERROR"); //se tutto errato
	}
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*4) );  // quinta riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);



		if (bestCombo == libreriaCanzoni[songSelection]->length){

			ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*6) ); // settima riga
			snprintf(buffer_schermo, sizeof(buffer_schermo), "Full combo!");
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);

			if (perfect == libreriaCanzoni[songSelection]->length){

				ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*7) ); // ottava riga
				snprintf (buffer_schermo, sizeof(buffer_schermo), "all perfect!");
				ssd1306_WriteString(buffer_schermo,Font_7x10, White);
			}
		}

		ssd1306_UpdateScreen();

	HAL_Delay(5000);
		//ssd1306_Fill(Black);       // Svuota la memoria cancellando tutti i pixel
		//ssd1306_DrawRectangle(0, 0, 127, 127, White);
		//ssd1306_UpdateScreen();    // Invia il comando allo schermo fisico per spegnerlo
	clean_screen();
	HAL_Delay(200);


	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*0) ); // prima riga
	ssd1306_WriteString("RESULTS DETAILS:",Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "PERFECT: %i", perfect);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "GREAT: %i", great);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ); // terza riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "GOOD: %i", good);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*3) ); // quarta riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "BAD: %i", bad);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*4) ); // quinta riga
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	snprintf(buffer_schermo, sizeof(buffer_schermo), "MISS: %i", miss);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*5) ); // sesta riga
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
    //ssd1306_Fill(Black);
    //ssd1306_DrawRectangle(0, 0, 127, 127, White);
    clean_screen();

    ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (TITLE_Y + TITLE_SPACE)*0) ); // prima riga
    ssd1306_WriteString("RHYTHM GAME",Font_11x18, White);

    ssd1306_SetCursor(14, (ORIGIN_Y + (TITLE_Y + TITLE_SPACE)*1) ); // seconda riga
    ssd1306_WriteString("PROTOTYPE",Font_11x18, White);
    ssd1306_UpdateScreen();
}

void clean_screen(void){ //così invece che scrivere 3 righe ne scrivo una sola
	ssd1306_Fill(Black);
	ssd1306_DrawRectangle(0, 0, 127, 127, White);
	//ssd1306_UpdateScreen();
	ssd1306_UpdateScreen_DMA();
}

void score_screen_print(char* songToPlay, char* feedback){
	//clean_screen(); //pulisco ogni volta
	ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*3) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*3 + TITLE_Y), Black); // cancello feedback
	ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*6) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*7 + FONT_Y), Black); // cancello feedback


	//canzone da fare
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*0) ); // prima riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", songToPlay);
	//ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	ssd1306_WriteString_DMA(buffer_schermo,Font_7x10, White);

	//feedback
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*3) ); // quarta riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", feedback);
	//ssd1306_WriteString(buffer_schermo,Font_11x18, White);
	ssd1306_WriteString_DMA(buffer_schermo,Font_7x10, White);
	//combo
	snprintf(buffer_schermo, sizeof(buffer_schermo), "COMBO: %i", combo);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*6) ); // settima riga
	//ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	ssd1306_WriteString_DMA(buffer_schermo,Font_7x10, White);

	//score
	snprintf(buffer_schermo, sizeof(buffer_schermo), "SCORE: %i", score);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*7) ); // ottava riga
	//ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	ssd1306_WriteString_DMA(buffer_schermo,Font_7x10, White);


	//ssd1306_UpdateScreen(); //vedo se spostare alla fine di gameTask nel loop
	ssd1306_UpdateScreen_DMA();

}


