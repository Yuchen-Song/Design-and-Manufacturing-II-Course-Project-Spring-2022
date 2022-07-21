#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
	

//motor and Timer init
void F_MOTOR_Init(void);
void B_MOTOR_Init(void);


//�����������Ļ�������
void MOTOR_A(int state);
void MOTOR_B(int state);
void MOTOR_C(int state);
void MOTOR_D(int state);

//С���˶�����
void Car_Go(void);
void Car_Back(void);
void Car_Stop(void);
void Car_Turn_Left(void);
void Car_Turn_Right(void);
void Car_Move_Left(void);
void Car_Move_Right(void);
void Car_Turn_lLeft(void);
void Car_Turn_lRight(void);
void setcompare(int Lpwm,int Rpwm);
void pwm_control(int Lpwm,int Rpwm);
#endif

