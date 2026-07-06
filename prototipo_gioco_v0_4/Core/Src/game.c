/*
 * game.c
 *
 *  Created on: 23 apr 2026
 *      Author: elyca
 */
#include <main.h>
#include "constants.h"
#include "game.h"
#include "cmsis_os.h"
#include "buzzer.h"

int combo = 0; //tiene conto la combo
int score = 0; //tiene conto dello score
int perfect = 0;
int great = 0;
int good = 0;
int bad = 0;
int miss = 0;
int bestCombo = 0; //to keep track of the best combo
float avgResponseTime = 0; //to make average response time later
int hitNotes = 0;  // to sum all hit notes and make average


extern uint8_t buttonPressed ; // 0 se non premuto, 1 se premuto
extern uint8_t timeoutOccurred ; // missed input
extern int8_t pressedButtonIndex ; //to understand which button has been pressed
extern uint8_t targetIndex;
extern const GameNote_t* melody;
extern uint32_t startTime;
extern osTimerId TimeoutTimerHandle;
extern uint32_t reactionTime;
extern TIM_HandleTypeDef htim2;
extern uint8_t songSelection;

//my songs
extern const GameNote_t superMario[MELODY_LENGTH];
extern const GameNote_t starWars[MELODY_LENGTH];
extern const GameNote_t happyBirthday[MELODY_LENGTH];
extern const GameNote_t jingleBells[MELODY_LENGTH];
extern const GameNote_t innoAllaGioia[MELODY_LENGTH];


typedef struct {  //struct with port and pin associated
    GPIO_TypeDef* port;
    uint16_t pin;
   // uint16_t frequency; //for generating a note with the buzzer
} HardwareElement_t;

const GameNote_t* libreriaCanzoni[] = {
    superMario,
    starWars,
    happyBirthday,
    jingleBells,
    innoAllaGioia
};


#define NUMERO_CANZONI (sizeof(libreriaCanzoni) / sizeof(libreriaCanzoni[0]))

const char* nomiCanzoni[NUMERO_CANZONI] = { // ho messo questo per dire pure come si chiama la canzone poi
    "Super Mario",
    "Star Wars",
    "Tanti Auguri",
    "Jingle Bells",
    "Inno alla Gioia"
};

extern HardwareElement_t array_leds[NUM_BUTTONS];
extern HardwareElement_t array_bottoni[NUM_BUTTONS];



GameNote_t* melodySelection(){
	printf("---------------------Song Selection Menu:-----------------------\r\n");
	printf("Press yellow button to go back, green to go on and red to select\r\n");
	GameNote_t* selectedSong;

	while(1) {
		if (buttonPressed == 1){ //messo così alrimenti prendeva PURE SE SCHIACCIAVO IL BLU

			if (pressedButtonIndex == 2) { //indietro di uno

				if (songSelection == 0)
					songSelection = NUMERO_CANZONI-1;
				else
					songSelection--;


			}

			if (pressedButtonIndex == 0) { // avanti di 1

				if (songSelection == NUMERO_CANZONI-1)
					songSelection = 0;
				else
					songSelection++;

			}

			if (pressedButtonIndex == 1) { //selezione
				break;
			}
			osDelay(200); //per evitare debounce  bottonui
			printf("Song %u: %s \r\n", songSelection, nomiCanzoni[songSelection]); //cosi mi dice pure come si chiama la canzone
			buttonPressed = 0; //lo ho spostato qui
		}
		osDelay(10); //altro delay poer rtos
		//buttonPressed = 0;
	}

	printf("Song %u selected: %s will now start: \r\n", songSelection, nomiCanzoni[songSelection]);

	selectedSong = libreriaCanzoni[songSelection];

	return selectedSong;

}


// Funzione magica per generare una nota
void PlayTone(uint16_t freq) {
    if (freq == 0) return; // Se è 0, non suono

    // La matematica del suono: il timer va a 1 MHz (1.000.000 Hz)
    // Per avere 440 Hz, devo fargli contare fino a (1000000 / 440)
    uint32_t array_value = 1000000 / freq;

    __HAL_TIM_SET_AUTORELOAD(&htim2, array_value);     // Imposta l'altezza della nota, &htim2 è il nome del timer
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, array_value / VOLUME); // Imposta il volume (Duty cycle 50%)

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);        // Accendi il suono!
}

// Funzione per spegnere il suono
void StopTone() {
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

void scoreEvaluate(int timer_value){ //per valutare
    if (timer_value == MISSVALUE){
        combo = 0;
        printf("miss.\r\n");
        miss++;
    }
    else{
    	avgResponseTime += timer_value; //to do average at the end
    	hitNotes++;

     if(timer_value < PERFECT_TRESHOLD * DIFFICULTY){
     	printf("PERFECT\r\n");
     	score += 500;
     	combo++;
     	perfect++;
     }
     else if(timer_value < PERFECT_TRESHOLD*2 * DIFFICULTY){
     	printf("GREAT\r\n");
     	score += 300;
     	combo++;
     	great++;
     }
     else if(timer_value < PERFECT_TRESHOLD*3 * DIFFICULTY){
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
	hitNotes = 0;
}


void gamePlay(void){
	  for(;;)
	  {
		  combo = 0;
		  score = 0;
	      printf("\r\n=== RHYTHM GAME PROTOTYPE ===\r\n");
	      melody = melodySelection();
	      printf("Press the BLUE button to start\r\n");

	      buttonPressed = 0;

	      // 1. Fase di attesa inizio
	      while (buttonPressed != BLUE_BUTTON){ //prova qui per baco
	    	  if (buttonPressed != 0 && buttonPressed != BLUE_BUTTON) { //PROVA, ALTRIMENTI SI IMPALLA
	    	            printf("Wrong button! Press the start button.\r\n");
	    	            buttonPressed = 0;
	    	       }
	          osDelay(200); // Usa osDelay per non impallare FreeRTOS (AUMENTATO ALTRIMENTI STAMPAVA PIU VOLTE LA RIGA NELL'IF)
	      }

	      buttonPressed = 0; // Resetta per il gioco
	      printf("\r\nGame start! \r\n");

	      uint8_t currentNoteIndex = 0; //to keep track of the melody

	      // 2. Ciclo della Canzone
	      for(int i = 0; i < SONGLENGHT; i++){
	    	  srand(osKernelSysTick());
	    	  targetIndex = rand() % NUM_BUTTONS;
	    	  HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	    	  StopTone(); //stops the note
	          printf("\r\nWAIT FOR THE LED...\r\n");

	          // Pausa casuale
	        //  osDelay((rand() % SECONDS) + SECONDS_OFFSET);
	          // 1. Aspetta il tempo dettato dallo spartito per questa specifica nota
	                 osDelay(melody[currentNoteIndex].waitTime  * DIFFICULTY);
	          // Prepariamo le variabili per questo round
	          buttonPressed = 0;
	                    timeoutOccurred = 0;
	                    pressedButtonIndex = -1;

	                    // 1. Estrai un numero a caso tra 0 e (NUM_BOTTONI - 1)
	                    //targetIndex = rand() % NUM_BUTTONS;

	                    // 2. Accendi SOLO il led scelto dall'array!
	                    HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_SET);
	                   // PlayTone(melody[currentNoteIndex]); //plays the note
	                    PlayTone(melody[currentNoteIndex].frequency);
	                    currentNoteIndex ++; //increment
	                    if (currentNoteIndex >= MELODY_LENGTH) {
	                                  currentNoteIndex = 0;
	                        }
	                    printf("PREMI IL BOTTONE %d!\r\n", targetIndex);

	                    startTime = osKernelSysTick();
	                    osTimerStart(TimeoutTimerHandle, 2000 * DIFFICULTY);

	                    // 3. Aspetta il click...
	                    while(buttonPressed == 0 && timeoutOccurred == 0) {
	                        osDelay(1);
	                    }

	                    // 4. Spegni il led che era acceso
	                    //HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                    //StopTone(); //stops the note
	                    osTimerStop(TimeoutTimerHandle);

	                    // 5. Valutazione: ha premuto il tasto, ma è quello GIUSTO?
	                    if (buttonPressed == 1) {
	                        if (pressedButtonIndex == targetIndex) {
	                        	pressedButtonIndex = BLUE_BUTTON; // iniziando hai premuto il tatso blu, dopo ne premi un'altro. Se non va bene rimanere con altri tasti in memoria, al momento torniamo con il blu solo dopo aver capito che il tasto premuto è giusto.
	                            // HA PREMUTO IL TASTO GIUSTO!
	                            printf("GIUSTO! Preso in: %lu ms\r\n", reactionTime);
	                             scoreEvaluate(reactionTime);
	                        } else {
	                            // HA SBAGLIATO TASTO!
	                            printf("ERRORE! Hai premuto il tasto sbagliato!\r\n");
	                             scoreEvaluate(MISSVALUE);
	                        }
	                    }
	                    else if (timeoutOccurred == 1) {
	                        printf("MANCATO! Troppo lento.\r\n");
	                         scoreEvaluate(MISSVALUE);
	                         pressedButtonIndex = targetIndex; //
	                         //timeoutOccurred = 0; //AGGIUNTO
	                         //StopTone(); //se qui c'è baco
	                    }
	                    else if (buttonPressed == BLUE_BUTTON){ //tasto blu riparte il gioco
	                    	printf("Il gioco ripartirà a breve...\r\n");
	                    	resetScore(); //resetto tutto per far iniziare il gioco da capo
	                    	currentNoteIndex = 0;
	                    	i = -1; //altrimenti max era 14
	                    	osDelay(1000);
	                    }
	                    else{ //per errori vari random
	                        printf("ERRORE! Hai premuto un tasto non riconosciuto!\r\n");
	                        scoreEvaluate(MISSVALUE);
	                    }
	                   // osDelay(SPACE_BETWEEN_NOTES); // Pausa prima del prossimo LED
	                    if(timeoutOccurred == 0 && reactionTime < (melody[currentNoteIndex].waitTime  * DIFFICULTY)){
	                    	//printf("%u \n", (melody[currentNoteIndex].waitTime* DIFFICULTY - reactionTime));
	                    	HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                    	osDelay((melody[currentNoteIndex].waitTime  * DIFFICULTY) - reactionTime);
	                          }
	                    HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                    StopTone(); //stops the note
	      }
	                          printf("\r\n------GAME OVER, THANKS FOR PLAYING!------\r\n");
	                          //HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                          //StopTone(); //stops the note
	                          finalScore(); //print final results
	                          osDelay(3000); // Aspetta un po' prima di permettere di rigiocare
	  }
}
