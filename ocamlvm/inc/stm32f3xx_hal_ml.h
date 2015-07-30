#ifdef NUCLEO
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#endif
#include <caml/mlvalues.h>

value HalInit                 (void);
value HalDeInit               (void);
value HalMspInit              (void);
value HalMspDeInit            (void);
value HalInitTick             (int TickPriority);
value HalIncTick              (void);
value HalDelay                (int Delay);
value HalSuspendTick          (void);
value HalResumeTick           (void);
value HalGetTick              (void);
value HalGetHalVersion        (void);
value HalGetREVID             (void);
value HalGetDEVID             (void);
value HalEnableDBGSleepMode   (void);
value HalDisableDBGSleepMode  (void);
value HalEnableDBGStopMode    (void);
value HalDisableDBGStopMode   (void);
value HalEnableDBGStandbyMode (void);
value HalDisableDBGStandbyMode(void);
value BspGetVersion           (void);
value BspLedInit              (int Led);
value BspLedOn                (int Led);
value BspLedOff               (int Led);
value BspLedToggle            (int Led);
value BspPbInit               (int Button, int ButtonMode);
value BspPbGetState           (int Button);
