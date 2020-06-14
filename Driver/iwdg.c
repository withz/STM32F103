#include "iwdg.h"

void IWDG_Config(uint8_t prv ,uint16_t rlv)
{
    // USE LSI,  @arg IWDG_Prescaler_4 - 256
    // Example: IWDG_Prescaler_64 ,625
    IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
    IWDG_SetPrescaler( prv );
    IWDG_SetReload( rlv );
    IWDG_ReloadCounter();
    IWDG_Enable();    
}

void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}


