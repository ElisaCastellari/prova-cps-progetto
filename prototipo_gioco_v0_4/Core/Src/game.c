/*
 * game.c
 *
 *  Created on: 23 apr 2026
 *      Author: elyca
 */
#include <main.h>
#include <string.h>
#include "constants.h"
#include "game.h"
#include "cmsis_os.h"
#include "buzzer.h"
#include "hm10_ble.h"


int reactionTime_screen = 0;


int combo = 0; //tiene conto la combo
volatile int score = 0; //tiene conto dello score
int perfect = 0;
int great = 0;
int good = 0;
int bad = 0;
int miss = 0;
int bestCombo = 0; //to keep track of the best combo
float avgResponseTime = 0; //to make average response time later
int hitNotes = 0;  // to sum all hit notes and make average

char* songName; //per passare il nome del song
char* noteFeedback; //per dire quanto bene ho fatto


extern uint8_t buttonPressed ; // 0 se non premuto, 1 se premuto
extern volatile uint8_t timeoutOccurred ; // missed input
extern int8_t pressedButtonIndex ; //to understand which button has been pressed
extern uint8_t targetIndex;
extern const GameNote_t* melody;
extern uint32_t startTime;
extern osTimerId TimeoutTimerHandle;
extern uint32_t reactionTime;
extern TIM_HandleTypeDef htim2;
extern uint8_t songSelection;
extern uint8_t rx_byte;
extern UART_HandleTypeDef huart1;

extern volatile uint8_t recieved;

//my songs
extern const Song_t superMario_song;
extern const Song_t starWars_song;
extern const Song_t happyBirthday_song;
extern const Song_t jingleBells_song;
extern const Song_t innoAllaGioia_song;
extern const Song_t superMarioExtended_song;
extern const Song_t vitaSpericolata_song;

extern char buffer_schermo[128];


typedef struct {  //struct with port and pin associated
    GPIO_TypeDef* port;
    uint16_t pin;
   // uint16_t frequency; //for generating a note with the buzzer
} HardwareElement_t;

const Song_t* libreriaCanzoni[] = {
    &superMario_song,
    &starWars_song,
    &happyBirthday_song,
    &jingleBells_song,
    &innoAllaGioia_song,
	&superMarioExtended_song,
	&vitaSpericolata_song
};

extern volatile uint8_t recievedOK;


#define NUMERO_CANZONI (sizeof(libreriaCanzoni) / sizeof(libreriaCanzoni[0]))

/*
const char* nomiCanzoni[NUMERO_CANZONI] = { // ho messo questo per dire pure come si chiama la canzone poi
    "Super Mario",
    "Star Wars",
    "Tanti Auguri",
    "Jingle Bells",
    "Inno alla Gioia",
	"superMarioExtended"
}; */

extern HardwareElement_t array_leds[NUM_BUTTONS];
extern HardwareElement_t array_bottoni[NUM_BUTTONS];



GameNote_t* melodySelection(){

	//////////////////ble recieve prova//////////////////////////
	//int rec = 1;
	timeoutOccurred = 0;
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    osTimerStart(TimeoutTimerHandle, 10000);

	while(1){
	if (recievedOK == 1){
//<<<<<<< HEAD

//=======
		//rec = bt_recieve_int();
//>>>>>>> f5508766f5717d4afd332a2be6659633f5bf6d32
		break;
	}
    if(timeoutOccurred == 1){
    	break;
    	}
    osDelay(10);
	}

	recieved = 0; //resetto le mie variabili
	timeoutOccurred = 0;
	sprintf(buffer_schermo, "Difficulty is: %u \r\n", rx_byte); //lo scrivo qui sfrutto sempre il buffer dello schermo per non istanziarne un altro
	//HAL_UART_Transmit(&huart2, (uint8_t*)buffer_schermo, strlen(buffer_schermo), 100);
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer_schermo, strlen(buffer_schermo), 100);
	osDelay(300); //gli do un po di tempo
	//rx_byte = '0';
	//////////////////////////////////////////////////////////////

	clean_screen();

	printf("---------------------Song Selection Menu:-----------------------\r\n");
	printf("Press yellow button to go back, green to go on and red to select\r\n");


	///////////////////////prova ble//////////////////////////////////


	/////////////////////////////////////////////////////////////////

	//screen
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*0) ); // prima riga
	ssd1306_WriteString("SONG SELECTION:",Font_7x10, White);

	GameNote_t* selectedSong;
	songSelection = 0;
	pressedButtonIndex = -1; //per evitare click involontari prima
	buttonPressed = 0;

	//screen
	ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1 + FONT_Y), Black); // cancello seconda riga

	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "Song %u:", songSelection + 1);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2 + FONT_Y), Black); // cancello terza riga

	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2)); // terza riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", libreriaCanzoni[songSelection]->name);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	ssd1306_UpdateScreen();

	osDelay(200);
	printf("Song %u: %s \r\n", songSelection + 1, libreriaCanzoni[songSelection]->name); //cosi mi dice pure come si chiama la canzone
	while(1) {
		//pressedButtonIndex = -1;
		//printf("Song %u: %s \r\n", songSelection + 1, nomiCanzoni[songSelection]); //cosi mi dice pure come si chiama la canzone
		if (buttonPressed == 1){ //messo così alrimenti prendeva PURE SE SCHIACCIAVO IL BLU

			if (pressedButtonIndex == 2) { //indietro di uno

				if (songSelection == 0)
					songSelection = NUMERO_CANZONI-1;
				else
					songSelection--;


			} else if (pressedButtonIndex == 0) { // avanti di 1

				if (songSelection == NUMERO_CANZONI-1)
					songSelection = 0;
				else
					songSelection++;

			} else if (pressedButtonIndex == 1) { //selezione
				break;

			} else{
				printf("unrecognised input \r\n"); //per evitare bug
				buttonPressed = 0;
			}

			osDelay(200); //per evitare debounce  bottonui
			printf("Song %u: %s \r\n", songSelection + 1, libreriaCanzoni[songSelection]->name); //cosi mi dice pure come si chiama la canzone

			ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1 + FONT_Y), Black); // cancello seconda riga


			//messa per schermetto
			ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga
			snprintf(buffer_schermo, sizeof(buffer_schermo), "Song %u:", songSelection + 1);
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);

			ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2 + FONT_Y), Black); // cancello terza riga

			ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ); // terza riga
			snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", libreriaCanzoni[songSelection]->name);
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);
			ssd1306_UpdateScreen();

			osDelay(200);
			buttonPressed = 0; //lo ho spostato qui
			//pressedButtonIndex = -1;
		}
		else if (buttonPressed == BLUE_BUTTON){ //in caso in cui premo il blu. torna all'inizio della lista
			//printf("\n\n\n sono qui \r\n\n\n");
			songSelection = 0;
			buttonPressed = 0;
			osDelay(200);
			printf("Song %u: %s \r\n", songSelection + 1, libreriaCanzoni[songSelection]->name);

			ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*1 + FONT_Y), Black); // cancello seconda riga


			//screen
			ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga
			snprintf(buffer_schermo, sizeof(buffer_schermo), "Song %u:", songSelection + 1);
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);

			ssd1306_FillRectangle(ORIGIN_X ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ,126 ,(ORIGIN_Y + (FONT_Y + ROW_SPACE)*2 + FONT_Y), Black); // cancello terza riga

			ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ); // terza riga
			snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", libreriaCanzoni[songSelection]->name);
			ssd1306_WriteString(buffer_schermo,Font_7x10, White);
			ssd1306_UpdateScreen();

		}
		else if (buttonPressed != 0){
			printf("unrecognised input \r\n");
			buttonPressed = 0; //per evitare i bug
			osDelay(200);
		}
		osDelay(10); //altro delay poer rtos
		//buttonPressed = 0;
	}

	songName = libreriaCanzoni[songSelection]->name;
	printf("Song %u selected: %s will now start: \r\n", songSelection + 1 , libreriaCanzoni[songSelection]->name);

	//screen
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*1) ); // seconda riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "Song %u selected:", songSelection + 1);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);

	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*2) ); // terza riga
	snprintf(buffer_schermo, sizeof(buffer_schermo), "%s", libreriaCanzoni[songSelection]->name);
	ssd1306_WriteString(buffer_schermo,Font_7x10, White);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*5) ); //sesta riga
	ssd1306_WriteString("Press BLUE button",Font_7x10, White);
	ssd1306_SetCursor(ORIGIN_X, (ORIGIN_Y + (FONT_Y + ROW_SPACE)*6) ); // settima riga
	ssd1306_WriteString("to start",Font_7x10, White);
	ssd1306_UpdateScreen();
	osDelay(200); //per evitare debounce  bottoni

	selectedSong = libreriaCanzoni[songSelection]->notes;

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
	//////////////////ble////////////////
	bt_transmit_int(timer_value);
	/////////////////////////////////////
    if (timer_value == MISSVALUE){
        combo = 0;
        printf("miss.\r\n");
        noteFeedback = "MISS";
        miss++;
    } else if (timer_value == SKIP_VALUE){
    	//NON FA NULLA
    	//printf("%\n\n\n index: %i \r\n\n\n\n", timer_value);
    	noteFeedback = "SKIPPED";
    }
    else{


    	avgResponseTime += timer_value; //to do average at the end
    	hitNotes++;

     if(timer_value < PERFECT_TRESHOLD * DIFFICULTY){
     	printf("PERFECT\r\n");
     	noteFeedback = "PERFECT";
     	score += 500;
     	combo++;
     	perfect++;
     }
     else if(timer_value < PERFECT_TRESHOLD*2 * DIFFICULTY){
     	printf("GREAT\r\n");
     	noteFeedback = "GREAT";
     	score += 300;
     	combo++;
     	great++;
     }
     else if(timer_value < PERFECT_TRESHOLD*3 * DIFFICULTY){
     	printf("GOOD\r\n");
     	noteFeedback = "GOOD";
     	score += 150;
     	combo++;
     	good++;
     }
     else{
     	printf("BAD\r\n");
     	noteFeedback = "BAD";
     	combo = 0;
     	score += 50;
     	bad++;
     }
    }
	if (combo >= bestCombo){ //for later evaluations of the combo
		bestCombo = combo;
	}

	//screen
	 score_screen_print(songName, noteFeedback);


	 ////////////////////time evaluation////////////////////////////////////7
	 //osDelay(100);
	 reactionTime_screen = osKernelSysTick() - reactionTime_screen;
	 printf("\n\n\n\n\n  reaction time screen + printf = %i", reactionTime_screen);
	 //////////////////////////////////////////////////////////////


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

	if(hitNotes != 0){ //per evitare divisioni per zero
	printf("Average response time: %i\r\n", (int)(avgResponseTime / hitNotes)); //average response time, casted as int for semplicity
	bt_transmit_int((int)(avgResponseTime / hitNotes));
	} else {
		printf("It was not possible to evaluate your response time \r\n");
	}

	if (bestCombo == libreriaCanzoni[songSelection]->length){
		printf("Full combo! \r\n");
		if (perfect == libreriaCanzoni[songSelection]->length){
			printf ("all perfect!\r\n");
		}
	}
	printf("---------------------------------------------------\r\n");
	//resetScore(); // i reset to be ready for a new game
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
	          osDelay(300); // Usa osDelay per non impallare FreeRTOS (AUMENTATO ALTRIMENTI STAMPAVA PIU VOLTE LA RIGA NELL'IF)
	      }

	      buttonPressed = 0; // Resetta per il gioco
	      printf("\r\nGame start! \r\n");
	      clean_screen();
	      uint8_t currentNoteIndex = 0; //to keep track of the melody

	      // 2. Ciclo della Canzone
	      for(int i = 0; i < libreriaCanzoni[songSelection]->length; i++){
	    	  printf("\n\n\n %i \n\n\n", (int)libreriaCanzoni[songSelection]->length);
	    	  srand(osKernelSysTick());

	    	  if ((i % SKIP_NOTES) == 0){
	    		  targetIndex = rand() % NUM_BUTTONS;
	    	  } else targetIndex = SKIP_VALUE;
	    	  //targetIndex = rand() % NUM_BUTTONS;
	    	  HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	    	  StopTone(); //stops the note
	          printf("\r\nWAIT FOR THE LED...\r\n");

	          // Pausa casuale
	        //  osDelay((rand() % SECONDS) + SECONDS_OFFSET);
	          // 1. Aspetta il tempo dettato dallo spartito per questa specifica nota
	                // osDelay(melody[currentNoteIndex].waitTime  * DIFFICULTY);
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
	                    if (currentNoteIndex >= libreriaCanzoni[songSelection]->length) {
	                                  currentNoteIndex = 0;
	                        }
	                    printf("PREMI IL BOTTONE %d!\r\n", targetIndex);

	                    startTime = osKernelSysTick();
	                    osTimerStart(TimeoutTimerHandle, melody[currentNoteIndex-1].waitTime * DIFFICULTY);

	                    // 3. Aspetta il click...
	                    while(buttonPressed == 0 && timeoutOccurred == 0) {
	                        osDelay(1);
	                    }

	                    // 4. Spegni il led che era acceso
	                    //HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                    //StopTone(); //stops the note
	                    osTimerStop(TimeoutTimerHandle);

	                    // 5. Valutazione: ha premuto il tasto, ma è quello GIUSTO?
	                    if (targetIndex == SKIP_VALUE){
	                    	scoreEvaluate(SKIP_VALUE);
	                    }
	                    else if (buttonPressed == 1) {


	                    	//per calcolare tempo schemro
	                    	reactionTime_screen = osKernelSysTick();

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
	                         //timeoutOccurred = 0; //AGGIUNTO di nuovo (9/7)
	                         scoreEvaluate(MISSVALUE);
	                         pressedButtonIndex = targetIndex; //
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
	                    //timeoutOccurred = 0; //prova messo qui
	                    HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                    StopTone(); //stops the note
	      }
	                          printf("\r\n------GAME OVER, THANKS FOR PLAYING!------\r\n");
	                          //HAL_GPIO_WritePin(array_leds[targetIndex].port, array_leds[targetIndex].pin, GPIO_PIN_RESET);
	                          //StopTone(); //stops the note
	                          finalScore_screen(); //con lo screen
	                          osDelay(1000);
	                          finalScore(); //print final results
	                          osDelay(3000); // Aspetta un po' prima di permettere di rigiocare
	                          resetScore();
	  }
}
