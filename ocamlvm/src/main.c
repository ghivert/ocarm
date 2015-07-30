#include "caml/startup.h"
#include <stdlib.h>
//#include "stm32f3xx_hal.h"
//#include "stm32f3_discovery.h"

int main (int argc, char* argv[]) {
  //HAL_Init();
  //BSP_LED_Init(LED3);
  //BSP_LED_On(LED3);
  caml_main(argv);
  //BSP_LED_Off(LED3);
  return 0;
}
