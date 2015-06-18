#include "stm32f3_dicovery_led.h"

void light_led_number (int number) {
  static int done = 0;
  if (done == 0) {
    HAL_Init();

    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_LED_Init(LED6);
    BSP_LED_Init(LED7);
    BSP_LED_Init(LED8);
    BSP_LED_Init(LED9);
    BSP_LED_Init(LED10);
    done++;
  }
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
  BSP_LED_Off(LED7);
  BSP_LED_Off(LED8);
  BSP_LED_Off(LED9);
  BSP_LED_Off(LED10);

  if (number & 0x1)
    BSP_LED_On(LED3);
  if (number & 0x2)
    BSP_LED_On(LED5);
  if (number & 0x4)
    BSP_LED_On(LED7);
  if (number & 0x8)
    BSP_LED_On(LED9);
  if (number & 0x10)
    BSP_LED_ON(LED10);
  if (number & 0x20)
    BSP_LED_ON(LED8);
  if (number & 0x30)
    BSP_LED_ON(LED6);
  if (number & 0x40)
    BSP_LED_ON(LED4);
}

