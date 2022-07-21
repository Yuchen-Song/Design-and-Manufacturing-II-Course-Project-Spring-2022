#include "pid_turn.h"
#include "mpu6050.h"  
#include "i2c.h" 
#include "delay.h"
#include "motor.h"
#include "usart.h"
void pi_init(pi * p)
{
	p->Kp = 18 ;
	p->Ki = 0.2 ;
	p->set  = 0.0;
	p->ekk = 0;
	p->inte = 0;
	p->Ur = 549.0;
}

float pi_cal(float yaw, pi *p)
{
	float ek=0,U_k=0;
	printf("YAW:%f\r\n\n",yaw);
	ek = (int)yaw - p->set;
	p->inte = p->inte + (ek + p->ekk)/2;
	U_k= p->Kp*ek  + p->Ki*p->inte;
  p->ekk=ek;
	if(U_k>(p->Ur))                                         
      U_k=p->Ur;
  if(U_k<-(p->Ur))
      U_k=-(p->Ur);
	return U_k;
}

void pi_turn()
{
  pi pi_s; float yaw_t,uk;int uk_i; int pwm_l = 0; int pwm_r = 0; float error = 0;
	pi_init(&pi_s);
	printf("GO into turning \r\n\n");
	yaw_t = MPU6050_Get_Yaw();
	uk = pi_cal(yaw_t,&pi_s);
	uk_i = (int)uk;
		while(yaw_t > 3||yaw_t < -3)
	{
		yaw_t = MPU6050_Get_Yaw();
		uk = pi_cal(yaw_t,&pi_s);
		uk_i = (int)uk;
		printf("U_k = %d\r\n\n",uk_i);
		if (uk_i > 0)
		{ 
			pwm_l = 549-uk_i;
			Car_Turn_Left();
			
			TIM_SetCompare3(TIM4,pwm_l);     	  //PB8 修改比较值pwm //左
			TIM_SetCompare4(TIM4,pwm_l);       	//PB9 修改比较值pwm //右
			//后轮
			TIM_SetCompare3(TIM3,pwm_l);    	//PB0 修改比较值pwm //左
			TIM_SetCompare4(TIM3,pwm_l);
			printf("PWM= %d\r\n\n",pwm_l);
			delay_ms(10);
			Car_Stop();
			delay_ms(10);
		}
		else if (uk_i < 0)
		{
			Car_Turn_Right();
			pwm_r = 549+uk_i;
			TIM_SetCompare3(TIM4,pwm_r);     	  //PB8 修改比较值pwm //左
			TIM_SetCompare4(TIM4,pwm_r);       	//PB9 修改比较值pwm //右
			//后轮
			TIM_SetCompare3(TIM3,pwm_r);    	//PB0 修改比较值pwm //左
			TIM_SetCompare4(TIM3,pwm_r);
			printf("PWM= %d\r\n\n",pwm_r);
			delay_ms(10);
			Car_Stop();
			delay_ms(10);
		}
	
	
	}
	
	
}