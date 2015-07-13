external halInit                  : unit -> unit = "HalInit"
external halDeInit                : unit -> unit = "HalDeInit"
external halMSP_Init              : unit -> unit = "HalMspInit"	      
external halMSP_DeInit            : unit -> unit = "HalMspDeInit"            
external halInitTick              : int  -> unit = "HalInitTick"             
external halIncTick               : unit -> unit = "HalIncTick"              
external halDelay                 : int  -> unit = "HalDelay"                
external halSuspendTick           : unit -> unit = "HalSuspendTick"	      
external halResumeTick            : unit -> unit = "HalResumeTick"           
external halGetTick               : unit -> int  = "HalGetTick"              
external halGetHalVersion         : unit -> int  = "HalGetHalVersion"        
external halGetREVID              : unit -> int  = "HalGetREVID"             
external halGetDEVID              : unit -> int  = "HalGetDEVID"             
external halEnableDBGSleepMode    : unit -> unit = "HalEnableDBGSleepMode"   
external halDisableDBGSleepMode   : unit -> unit = "HalDisableDBGSleepMode"  
external halEnableDBGStopMode     : unit -> unit = "HalEnableDBGStopMode"    
external halDisableDBGStopMode    : unit -> unit = "HalDisableDBGStopMode"   
external halEnableDBGStandbyMode  : unit -> unit = "HalEnableDBGStandbyMode" 
external halDisableDBGStandbyMode : unit -> unit = "HalDisableDBGStandbyMode"
external bspGetVersion            : unit -> int  = "BspGetVersion"  	      
external bspLedInit               : int  -> unit = "BspLedInit"              
external bspLedOn                 : int  -> unit = "BspLedOn"                
external bspLedOff                : int  -> unit = "BspLedOff"               
external bspLedToggle             : int  -> unit = "BspLedToggle"            
external bspPbInit                : int  -> int -> unit = "BspPbInit"
external bspPbGetState            : int  -> int  = "BspPbGetState"
