#include "stm32f3xx_hal_ml.h"

value HalInit(void) {
  HAL_Init();
}

value HalDeInit(void) {
  HAL_DeInit();
}

value HalMspInit(void) {
  HAL_MspInit();
}

value HalMspDeInit(void) {
  HAL_MspDeInit();
}
value HalInitTick(int TickPriority) {
  HAL_InitTick(Int_val(TickPriority));
}

value HalIncTick              (void) {
  HAL_IncTick();
}
value HalDelay                (int Delay) {
  HAL_Delay(Int_val(Delay));
}
value HalSuspendTick          (void) {
  HAL_SuspendTick          ();
}
value HalResumeTick           (void) {
  HAL_ResumeTick           ();
}
value HalGetTick              (void) {
  return HAL_GetTick              ();
}
value HalGetHalVersion        (void) {
  return HAL_GetHalVersion        ();
}
value HalGetREVID             (void) {
  return HAL_GetREVID             ();
}
value HalGetDEVID             (void) {
  return HAL_GetDEVID             ();
}
value HalEnableDBGSleepMode   (void) {
  HAL_EnableDBGSleepMode   ();
}
value HalDisableDBGSleepMode  (void) {
  HAL_DisableDBGSleepMode  ();
}
value HalEnableDBGStopMode    (void) {
  HAL_EnableDBGStopMode    ();
}
value HalDisableDBGStopMode   (void) {
  HAL_DisableDBGStopMode   ();
}
value HalEnableDBGStandbyMode (void) {
  HAL_EnableDBGStandbyMode ();
}
value HalDisableDBGStandbyMode(void) {
  HalDisableDBGStandbyMode();
}

value BspGetVersion(void) {
  return BSP_GetVersion();
}
value BspLedInit   (int Led) {
  Led = Int_val(Led);
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Init(Led);
}
value BspLedOn     (int Led) {
  Led = Int_val(Led);
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_On(Led);
}
value BspLedOff    (int Led) {
  Led = Int_val(Led);
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Off(Led);
}
value BspLedToggle (int Led) {
  Led = Int_val(Led);
  if (Led < 0 || Led > 7)
    invalid_argument("LED Non existent !");
  BSP_LED_Toggle(Led);
}
value BspPbInit    (int Button, int ButtonMode) {
  Button = Int_val(Button);
  ButtonMode = Int_val(ButtonMode);
  if (Button != 0)
    invalid_argument("Button not existent.");
  if (ButtonMode != 0 && ButtonMode != 1)
    invalid_argument("ButtonMode not existent.");
  BSP_PB_Init(Button, ButtonMode);
}
value BspPbGetState(int Button) {
  Button = Int_val(Button);
  if (Button != 0)
    invalid_argument("Button not existent.");
  return BSP_PB_GetState(Button);
}
