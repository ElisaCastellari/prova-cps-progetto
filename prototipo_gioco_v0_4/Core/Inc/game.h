/*
 * game.h
 *
 *  Created on: 23 apr 2026
 *      Author: elyca
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_


#include <main.h>
#include "buzzer.h"
#include "screen128x128.h"

void scoreEvaluate(int timer_value);
void resetScore(void);
void finalScore(void);
void gamePlay(void);
void StopTone(void);
void PlayTone(uint16_t freq);
GameNote_t* melodySelection();


#endif /* INC_GAME_H_ */
