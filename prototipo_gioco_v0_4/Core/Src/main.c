/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c -V0.0.4
  * @brief          : Main program body
  * author: Elisa Castellari 5610013 / Moreno Tomaghelli 5291133
  * prova push elisa
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "game.h"
#include "constants.h"
#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

osThreadId GameTaskHandle;
osTimerId TimeoutTimerHandle;
osTimerId myTimer02Handle;
/* USER CODE BEGIN PV */

typedef struct {  //struct with port and pin associated
    GPIO_TypeDef* port;
    uint16_t pin;
   // uint16_t frequency; //for generating a note with the buzzer
} HardwareElement_t;

HardwareElement_t array_leds[NUM_BUTTONS] = {
    {GPIOA, GPIO_PIN_5}, // LED 0
    {GPIOA, GPIO_PIN_6}, // LED 1
    {GPIOA, GPIO_PIN_7}  // LED 2
};

HardwareElement_t array_bottoni[NUM_BUTTONS] = {
    {GPIOB, GPIO_PIN_3}, // BUTTON 0
    {GPIOB, GPIO_PIN_4}, // BUTTON 1
    {GPIOB, GPIO_PIN_5}  // BUTTON 2
};


volatile uint8_t buttonPressed = 0; // 0 se non premuto, 1 se premuto
volatile uint8_t timeoutOccurred = 0; // missed input
volatile int8_t pressedButtonIndex = -1; //to understand which button has been pressed
volatile uint8_t songSelection = 0;
uint32_t startTime = 0;
uint8_t targetIndex = 0; //the led that is turned on
uint32_t reactionTime = 0;
GameNote_t* melody; //my melody

extern int combo; //tiene conto la combo
extern int score; //tiene conto dello score

//my songs
extern const GameNote_t superMario[MELODY_LENGTH];
extern const GameNote_t starWars[MELODY_LENGTH];
extern const GameNote_t happyBirthday[MELODY_LENGTH];
extern const GameNote_t jingleBells[MELODY_LENGTH];
extern const GameNote_t innoAllaGioia[MELODY_LENGTH];

const GameNote_t* libreriaCanzoni[] = {
    superMario,
    starWars,
    happyBirthday,
    jingleBells,
    innoAllaGioia
};

#define NUMERO_CANZONI (sizeof(libreriaCanzoni) / sizeof(libreriaCanzoni[0]))
//int timer_value = 0; // valuta tempo di risposta

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
void StartGameTask(void const * argument);
void TimeoutCallback(void const * argument);
void Callback02(void const * argument);

/* USER CODE BEGIN PFP */
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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

GameNote_t* melodySelection(){


	while(1) {
		if (buttonPressed){

			if (pressedButtonIndex == 2) {

				if (songSelection == 0)
					songSelection = NUMERO_CANZONI-1;
				else
					songSelection--;


			}

			if (pressedButtonIndex == 0) {

				if (songSelection == NUMERO_CANZONI-1)
					songSelection = 0;
				else
					songSelection++;

			}

			if (pressedButtonIndex == 1) {
				break;
			}
			osDelay(50);
			printf("Song %u \r\n", songSelection);

		}

		buttonPressed = 0;
	}

	printf("Song %u selected \r\n", songSelection);

	return libreriaCanzoni[songSelection];

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  // Leggiamo un valore a caso dalla memoria per inizializzare il random

  //printf("\r\n=== RHYTHM GAME PROTOTYPE ===\r\n");

  //-------------------------------------------------------------------------------
  //melodySelecion(melody);
  //melody = melodySelection();
  //song selection
  //-------------------------------------------------------------------------------
  //createDummySong();
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of TimeoutTimer */
  osTimerDef(TimeoutTimer, TimeoutCallback);
  TimeoutTimerHandle = osTimerCreate(osTimer(TimeoutTimer), osTimerOnce, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, Callback02);
  myTimer02Handle = osTimerCreate(osTimer(myTimer02), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of GameTask */
  osThreadDef(GameTask, StartGameTask, osPriorityNormal, 0, 1024); // impostata a mano a 1024 anzichè 128 sotto consiglio di gemini
  GameTaskHandle = osThreadCreate(osThread(GameTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_0_Pin|LED_1_Pin|LED_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_0_Pin LED_1_Pin LED_2_Pin */
  GPIO_InitStruct.Pin = LED_0_Pin|LED_1_Pin|LED_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_0_Pin BUTTON_1_Pin BUTTON_2_Pin */
  GPIO_InitStruct.Pin = BUTTON_0_Pin|BUTTON_1_Pin|BUTTON_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (buttonPressed == 0) {

	        // 1. IL TASTO BLU PER INIZIARE LA PARTITA!
	        if (GPIO_Pin == GPIO_PIN_13) {
	            buttonPressed = BLUE_BUTTON; // prova mia
	            return; // Esci subito dalla funzione, non fare altro
	        }

	        // 2. I TASTI COLORATI PER GIOCARE
	        for (int i = 0; i < NUM_BUTTONS; i++) {
	            if (GPIO_Pin == array_bottoni[i].pin) {
	                // Trovato! Calcolo il tempo
	                reactionTime = osKernelSysTick() - startTime; // start time viene eguagliato a osKernelSysTick() dopo ogni input
	                pressedButtonIndex = i; // Mi salvo QUALE ha premuto
	                buttonPressed = 1;      // Sblocco il gioco
	                break;                  // Esco dal ciclo for
	            }
	        }
	    }
	}

/* Callback del Timer (Fine del tempo) */
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartGameTask */
/**
  * @brief  Function implementing the GameTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGameTask */
void StartGameTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
//<<<<<<< HEAD
  for(;;)
  {
	  combo = 0;
	  score = 0;
      printf("\r\n=== RHYTHM GAME PROTOTYPE ===\r\n");

      melody = melodySelection();

      printf("Press the BLUE button to start\r\n");
//=======
//>>>>>>> 277fe622c39a7e296af40d7796e9d3ec46e9e35a

  /* USER CODE END 5 */
}

/* TimeoutCallback function */
void TimeoutCallback(void const * argument)
{
  /* USER CODE BEGIN TimeoutCallback */
	//printf("MANCATO! Troppo lento CALLBACK.\r\n"); //provo per debug a mettere nel callback
	timeoutOccurred = 1;
	//StopTone(); //resetto suono per andare avanti
  /* USER CODE END TimeoutCallback */
}

/* Callback02 function */
void Callback02(void const * argument)
{
  /* USER CODE BEGIN Callback02 */

  /* USER CODE END Callback02 */
}



/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  gamePlay();
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
