#ifndef _ENCODE_H
#define _ENCODE_H

#include "sys.h"
#define PWMA   TIM1->CCR1  //PA8
#define BIN1   PBout(13)
#define BIN2   PBout(12)
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的
//void Encode_Motor_Init(void);
//void Encode_PWM_Init(u16 arr,u16 psc);
//void Encode_Encoder_Init(void);
typedef struct
{
	float ekk;
	float Kp;
	float Kd;
	float set;
	int PWM;
}pd;

void pd_init(pd *);
float pd_cal(int pps, pd *p);

void Encoder_Init(void);
float Read_Encoder(int a);
void TIM2_IRQHandler(void);
void TIM_EncoderInterfaceConfig34(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity);
void Set_encoder_flag(int a);
void Set_speed_11(int set);
void Set_speed_12(int set);
void Set_speed_13(int set);
void Set_speed_14(int set);
void PD_Init(void);
#endif
