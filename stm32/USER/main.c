/*  工具  */
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "motor.h"

/*  避障  */
#include "encoder.h"
#include "sonic.h"
#include "avoid.h"
#include "i2c.h"  
#include "mpu6050.h"

/*  循迹  */
#include "usart3.h"  
#include "usart2.h"  

/*  抓取  */
#include "grasping.h"

extern int left1;
extern int right1;
extern int left2;
extern int right2;
extern u8 str2[8];


int main(void)
{ 
	float distance = 50.0;
	float yaw;
	int flag = 0;
	
	delay_init();	    
	uart_init(115200);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 
	F_MOTOR_Init();
	B_MOTOR_Init();
	printf("1\r\n");
	
	LED_Init();
  TIM3_PWM_Init(449,799); //预分频599。PWM频率=72000000/((599+1)*(599+1)) = 200 Hz (servo PWM频率)
	printf("2\r\n");
	
	USART3_Init(256000); 
	USART2_Init(256000); 
	printf("3\r\n");
	
	
	initPose(180);
	
	
	TIM5_Cap_Init(0XFFFF,72-1);	// TIM5ch1,ch2 (PA0, PA1)
	TIM4_Cap_Init(0XFFFF,72-1);	// TIM4ch3 (PB8)
	Encoder_Init();	// EXTI11,12,13,14 (PC11, PC12, PA13, PA14)
	HC_SR04_Init();	// (PA4, PC4, PC5)
//		MPU6050_Init();	// I2C1 (PB6, PB7)
//		MPU6050_EXTI_Init();	// EXTI5 (PA5)
	
	printf("START\r\n");
	
//	// 避障
//	delay_ms(1000);
////	Avoid();
//	delay_ms(1000);
//	
	
	delay_ms(300);
  //unloading();
	while(1)
	{   
//		distance = HC_SR04_GetDis(3);
//		if(distance < 15.0)
//		{
//			Car_Stop();
//			unloading();
//			delay_ms(1500);
//			break;
//		}
		if (flag==2) {
			TR2_Receive();
			printf("finishedfinishedfinishedfinishedfinishedfinished\r\n");
			//printf("lpwm%d rpwm%d",left2,right2);
			pwm_control(left2,right2);
			//printf("left2-flag2: %d\r\n",left2);
			delay_ms(20);
		}
		else if (flag==1) {
			printf("stopstopstopstopstopstopstop\r\n");
			Car_Stop();
			flag=graspingStage(flag);
			delay_ms(1000);
		} 
		else {
			TR2_Receive();
			//printf("moving\r\n");
			//printf("left2-else: %d\r\n",left2);
			pwm_control(left2,right2);
		}
		flag=graspingStage(flag);
		//printf("flag-out: %d\r\n",flag);
//    delay_ms(1000);

	}
}



