#include <stdint.h>
#include <stdio.h>
#include "stm32f3xx_hal_ml.h"

value HalInit(void) {
#ifdef NUCLEO
  HAL_Init();
#else
  return 0;
#endif
}

value HalDeInit(void) {
#ifdef NUCLEO
  HAL_DeInit();
#else
  return 0;
#endif
}

value HalMspInit(void) {
#ifdef NUCLEO
  HAL_MspInit();
#else
  return 0;
#endif
}

value HalMspDeInit(void) {
#ifdef NUCLEO
  HAL_MspDeInit();
#else
  return 0;
#endif
}
value HalInitTick(int TickPriority) {
#ifdef NUCLEO
  HAL_InitTick(Int_val(TickPriority));
#else
  return 0;
#endif
}

value HalIncTick              (void) {
#ifdef NUCLEO
  HAL_IncTick();
#else
  return 0;
#endif
}
value HalDelay                (int Delay) {
#ifdef NUCLEO
  HAL_Delay(Int_val(Delay));
#else
  return 0;
#endif
}
value HalSuspendTick          (void) {
#ifdef NUCLEO
  HAL_SuspendTick          ();
#else
  return 0;
#endif
}
value HalResumeTick           (void) {
#ifdef NUCLEO
  HAL_ResumeTick           ();
#else
  return 0;
#endif
}
value HalGetTick              (void) {
#ifdef NUCLEO
  return HAL_GetTick              ();
#else
  return 0;
#endif
}
value HalGetHalVersion        (void) {
#ifdef NUCLEO
  return HAL_GetHalVersion        ();
#else
  return 0;
#endif
}
value HalGetREVID             (void) {
#ifdef NUCLEO
  return HAL_GetREVID             ();
#else
  return 0;
#endif
}
value HalGetDEVID             (void) {
#ifdef NUCLEO
  return HAL_GetDEVID             ();
#else
  return 0;
#endif
}
value HalEnableDBGSleepMode   (void) {
#ifdef NUCLEO
  HAL_EnableDBGSleepMode   ();
#else
  return 0;
#endif
}
value HalDisableDBGSleepMode  (void) {
#ifdef NUCLEO
  HAL_DisableDBGSleepMode  ();
#else
  return 0;
#endif
}
value HalEnableDBGStopMode    (void) {
#ifdef NUCLEO
  HAL_EnableDBGStopMode    ();
#else
  return 0;
#endif
}
value HalDisableDBGStopMode   (void) {
#ifdef NUCLEO
  HAL_DisableDBGStopMode   ();
#else
  return 0;
#endif
}
value HalEnableDBGStandbyMode (void) {
#ifdef NUCLEO
  HAL_EnableDBGStandbyMode ();
#else
  return 0;
#endif
}
value HalDisableDBGStandbyMode(void) {
#ifdef NUCLEO
  HalDisableDBGStandbyMode();
#else
  return 0;
#endif
}

value BspGetVersion(void) {
#ifdef NUCLEO
  return BSP_GetVersion();
#else
  return 0;
#endif
}
value BspLedInit   (int Led) {
#ifdef NUCLEO
  Led = Int_val(Led) - 3;
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Init(Led);
#else
  return 0;
#endif
}
value BspLedOn     (int Led) {
#ifdef NUCLEO
  Led = Int_val(Led) - 3;
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_On(Led);
#else
  printf("J'allume la LED %d\n", Int_val(Led));
  return 0;
#endif
}
value BspLedOff    (int Led) {
#ifdef NUCLEO
  Led = Int_val(Led) - 3;
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Off(Led);
#else
  printf("J'eteins la LED %d\n", Int_val(Led));
  return 0;
#endif
}
value BspLedToggle (int Led) {
#ifdef NUCLEO
  Led = Int_val(Led) - 3;
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Toggle(Led);
#else
  printf("Je toggle la LED %d\n", Int_val(Led));
  return 0;
#endif
}
value BspPbInit    (int Button, int ButtonMode) {
#ifdef NUCLEO
  Button = Int_val(Button);
  ButtonMode = Int_val(ButtonMode);
  if (Button != 0)
    invalid_argument("Button not existent.");
  if (ButtonMode != 0 && ButtonMode != 1)
    invalid_argument("ButtonMode not existent.");
  BSP_PB_Init(Button, ButtonMode);
#else
  return 0;
#endif
}
value BspPbGetState(int Button) {
#ifdef NUCLEO
  Button = Int_val(Button);
  if (Button != 0)
    invalid_argument("Button not existent.");
  return BSP_PB_GetState(Button);
#else
  return 0;
#endif
}
