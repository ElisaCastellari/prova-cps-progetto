/*
 * game.c
 *
 *  Created on: 23 apr 2026
 *      Author: elyca
 */
#include "constants.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int combo = 0; //tiene conto la combo
int score = 0; //tiene conto dello score
int dummySong[SONGLENGHT]; //array con ritardi casuali

void scoreEvaluate(int timer_value){ //per valutare

     if(timer_value < PERFECT_TRESHOLD){
     	printf("PERFECT\r\n");
     	score += 500;
     	combo++;
     }
     else if(timer_value < PERFECT_TRESHOLD*2){
     	printf("GREAT\r\n");
     	score += 300;
     	combo++;
     }
     else if(timer_value < PERFECT_TRESHOLD*3){
     	printf("GOOD\r\n");
     	score += 150;
     	combo++;
     }
     else if (timer_value == MISSVALUE){
         combo = 0;
         printf("miss.\r\n");
     }
     else{
     	printf("BAD\r\n");
     	combo = 0;
     	score += 50;
     }
     printf("your combo is %i, score %i.\r\n", combo, score);

}

void createDummySong(void){ // gerera ritardi casuali per simulare una sequenza di una canzone
	 srand(42);

	 for(int i = 0; i < SONGLENGHT; i++)
		 dummySong[i] = rand() % SECONDS + SECONDS_OFFSET;
}

void finalScore(void){ //per printare i risultati completi a fine game
	//da fare
}


