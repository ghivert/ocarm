#include "main.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)
{
  
  HAL_Init();


  /* Configure the system clock to have a system clock = 72 Mhz */
  SystemClock_Config();

  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  BSP_LED_Init(LED7);
  BSP_LED_Init(LED8);
  BSP_LED_Init(LED9);
  BSP_LED_Init(LED10);
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  int was_pressed = 0; int indic = 0;
    
  while (1)
  {
       
     if (BSP_PB_GetState(BUTTON_USER)) {
      was_pressed =  !was_pressed;
      if (indic) {
        BSP_LED_Toggle(LED3);
        HAL_Delay(30);
        BSP_LED_Toggle(LED5);
        HAL_Delay(30);
        BSP_LED_Toggle(LED7);
        HAL_Delay(30);
        BSP_LED_Toggle(LED9);
        HAL_Delay(30);
        BSP_LED_Toggle(LED10);
        HAL_Delay(30);
        BSP_LED_Toggle(LED8);
        HAL_Delay(30);
        BSP_LED_Toggle(LED6);
        HAL_Delay(30);
        BSP_LED_Toggle(LED4);
      }
      if (!was_pressed)
        HAL_Delay(1000);
     }
   
    if (was_pressed) {
      indic = indic ? 0 : 1;
        BSP_LED_Toggle(LED3);
        HAL_Delay(30);
        BSP_LED_Toggle(LED5);
        HAL_Delay(30);
        BSP_LED_Toggle(LED7);
        HAL_Delay(30);
        BSP_LED_Toggle(LED9);
        HAL_Delay(30);
        BSP_LED_Toggle(LED10);
        HAL_Delay(30);
        BSP_LED_Toggle(LED8);
        HAL_Delay(30);
        BSP_LED_Toggle(LED6);
        HAL_Delay(30);
        BSP_LED_Toggle(LED4);
        HAL_Delay(30);        
    }
  }
  
  
  
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  //while(1)
  //{
  //}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  //while (1)
  //{
  //}
}
#endif
