#include "sys.h"

void TIM5_Cap_Init(u16 arr,u16 psc);

void TIM4_Cap_Init(u16 arr,u16 psc);

void TIM4_IRQHandler(void);

void TIM5_IRQHandler(void);

void HC_SR04_Init(void);

float HC_SR04_GetDis(u8 a);