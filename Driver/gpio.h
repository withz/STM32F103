#ifndef GPIO_H
#define GPIO_H

#include "stm32f10x.h"

typedef enum MG_PORT_t {
    MA, MB, MC, MD, ME, MF, MG
} MG_PORT;

typedef enum MG_MPIN_t {
    M0, M1, M2, M3, M4, M5, M6, M7,
    M8, M9, M10, M11, M12, M13, M14, M15
} MG_PIN;

void MG_Output(MG_PORT port, MG_PIN pin);
void MG_Input(MG_PORT port, MG_PIN pin, s8 mode_s);
void MG_Set(MG_PORT port, MG_PIN pin,u8 mode_s);
void MG_Toggle(MG_PORT port, MG_PIN pin);
u8 MG_Get(MG_PORT port, MG_PIN pin);
void MG_RCC(MG_PORT port);




#endif
