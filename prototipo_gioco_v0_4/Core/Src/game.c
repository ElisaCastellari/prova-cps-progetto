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
int perfect = 0;
int great = 0;
int good = 0;
int bad = 0;
int miss = 0;
int bestCombo = 0; //to keep track of the best combo
float avgResponseTime = 0; //to make average response time later
int hitNotes = 0;  // to sum all hit notes and make average

void scoreEvaluate(int timer_value){ //per valutare
    if (timer_value == MISSVALUE){
        combo = 0;
        printf("miss.\r\n");
        miss++;
    }
    else{
    	avgResponseTime += timer_value; //to do average at the end
    	hitNotes++;

     if(timer_value < PERFECT_TRESHOLD){
     	printf("PERFECT\r\n");
     	score += 500;
     	combo++;
     	perfect++;
     }
     else if(timer_value < PERFECT_TRESHOLD*2){
     	printf("GREAT\r\n");
     	score += 300;
     	combo++;
     	great++;
     }
     else if(timer_value < PERFECT_TRESHOLD*3){
     	printf("GOOD\r\n");
     	score += 150;
     	combo++;
     	good++;
     }
     else{
     	printf("BAD\r\n");
     	combo = 0;
     	score += 50;
     	bad++;
     }
    }
	if (combo >= bestCombo){ //for later evaluations of the combo
		bestCombo = combo;
	}
     printf("your combo is %i, score %i.\r\n", combo, score);

}

void createDummySong(void){ // gerera ritardi casuali per simulare una sequenza di una canzone
	 srand(42);

	 for(int i = 0; i < SONGLENGHT; i++)
		 dummySong[i] = rand() % SECONDS + SECONDS_OFFSET;
}

void finalScore(void){ //per printare i risultati completi a fine game
	printf("\n----------------final results:-------------------\r\n");
	printf("Score: %i\r\n", score);
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
}


void resetScore(void){ //to reset everything after the game has ended
	combo = 0;
	score = 0;
	bestCombo = 0;
	perfect = 0;
	great = 0;
	good = 0;
	bad = 0;
	miss = 0;
	avgResponseTime = 0;
}
