/*
 * screen128x128.h
 *
 *  Created on: 7 lug 2026
 *      Author: elyca
 */

#ifndef INC_SCREEN128X128_H_
#define INC_SCREEN128X128_H_

#include "ssd1306.h"
#include "ssd1306_fonts.h" // <-- Aggiunge i font (risolve l'errore 1)
#include "ssd1306_tests.h" // <-- Aggiunge le animazioni (risolve l'errore 2)
#include "game.h"
#include "constants.h"
#include "buzzer.h"
#include <stdio.h>

void finalScore_screen(void);
void screen_init(void);
void clean_screen(void);
void score_screen_print(char* songToPlay, char* feedback);

#endif /* INC_SCREEN128X128_H_ */
