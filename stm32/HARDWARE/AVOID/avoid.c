#include "delay.h"
#include "usart.h"

#include "sonic.h"
#include "encoder.h"
#include "motor.h"

#include "avoid.h"


void Avoid(void)
{
	// 避障部分
	float Distance, Distance_pre;	
	//int iter = 0;
	
	printf("Avoid Start!\r\n");
	
	Distance = HC_SR04_GetDis(1);
	Distance_pre = 0.0;
	Car_Stop();
	Set_encoder_flag(1);
	//前进
	while(1)
	{
		Car_Go();

		Set_speed_11(600);    	  //PB8 修改比较值pwm //左
		Set_speed_12(600);      	//PB9 修改比较值pwm //右
		//后轮
		Set_speed_13(600);   	//PB0 修改比较值pwm //左
		Set_speed_14(600);
		delay_ms(20);
		Distance = HC_SR04_GetDis(1);
		printf("Distance:%.2f\r\n",Distance);
		if((Distance < 17.0)&&(Distance > 3.0)) break;
	}
	Car_Stop();
	delay_ms(50);
	Distance = HC_SR04_GetDis(2);
	Distance_pre = Distance;
	//右移
	printf("RIGHT\r\n");
	while(1)
	{
		Car_Move_Right();
		Set_speed_11(600);    	  //PB8 修改比较值pwm //左
		Set_speed_12(600);      	//PB9 修改比较值pwm //右
		//后轮
		Set_speed_13(600);   	//PB0 修改比较值pwm //左
		Set_speed_14(600);
		delay_ms(10);
		Distance = HC_SR04_GetDis(2);
		if((Distance < 20.0)&&(Distance > 3.0)) break;
		Distance_pre = Distance;
	}
	Car_Stop();
	delay_ms(50);
		
	//姿态矫正
	//pi_turn();
	//delay_ms(100);
	//前进
	printf("GO\r\n");
	Distance = HC_SR04_GetDis(1);
	while(1)
	{
		Car_Go();
		Set_speed_11(600);    	  //PB8 修改比较值pwm //左
		Set_speed_12(600);      	//PB9 修改比较值pwm //右
		//后轮
		Set_speed_13(600);   	//PB0 修改比较值pwm //左
		Set_speed_14(600);
		delay_ms(10);
		Distance = HC_SR04_GetDis(1);
		if((Distance < 13.5)&&(Distance > 3.0)) break;
	}
	Car_Stop();
	delay_ms(50);
	Distance = HC_SR04_GetDis(3);
	Distance_pre = Distance;
	//左移
	printf("LEFT\r\n");
	while(1)
	{
//		if(iter >=0 && iter < 50)
//		{
		Car_Move_Left();
		Set_speed_11(600);    	  //PB8 修改比较值pwm //左
		Set_speed_12(600);      	//PB9 修改比较值pwm //右
		//后轮
		Set_speed_13(600);   	//PB0 修改比较值pwm //左
		Set_speed_14(600);
//		delay_ms(10);
//		}
//		else if(iter == 50)
//		{
//			pi_turn();
//			delay_ms(10);
//			iter =0;
//		}
//		iter +=1;
		Distance = HC_SR04_GetDis(3);
		if((Distance < 20.0)&&(Distance > 3.0)) break;
		Distance_pre = Distance;
	}
	Car_Stop();
	delay_ms(20);	
		//姿态矫正
//		pi_turn();
//		delay_ms(100);
	Car_Go();
	Set_speed_11(600);    	  //PB8 修改比较值pwm //左
	Set_speed_12(600);      	//PB9 修改比较值pwm //右
	//后轮
	Set_speed_13(600);   	//PB0 修改比较值pwm //左
	Set_speed_14(600);
	delay_ms(500);
	Car_Stop();
	Set_encoder_flag(0);	// 关闭编码器控制，避障阶段结束
	
}
