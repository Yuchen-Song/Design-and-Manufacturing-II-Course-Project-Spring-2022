#include "motor.h"
#include "delay.h"
#include "math.h"

int STOP=2;
int GO=1;
int BACK=0;

void F_MOTOR_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //①使能定时器 8 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM输出 PC8/PC9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //①初始化 GPIO
	
	  GPIO_SetBits(GPIOC,GPIO_Pin_8);//PC8置1
	  GPIO_SetBits(GPIOC,GPIO_Pin_9);//PC9置1
	
	  //两个电机的IO： PC0-3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //①初始化 GPIO
		
		//PC 0-3置0
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	  
		//初始化 TIM8
		TIM_TimeBaseStructure.TIM_Period = 899;  //设置在自动重装载周期值
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //设置预分频值
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//设置重复计数器为0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //③初始化 TIMx
		
		//初始化 TIM8 Channe3 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //④初始化外设 TIM8 OC3

		
		//初始化 TIM8 Channe4 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //④初始化外设 TIM8 OC4
		
		TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能预装载寄存器
		TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能预装载寄存器
		
		TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
		TIM_Cmd(TIM8, ENABLE);  //⑤使能 TIM8
		TIM_CtrlPWMOutputs(TIM8, ENABLE); // TIM8和普通定时器不同之处
		
}
void B_MOTOR_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //①使能定时器 3 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM输出 PC6/PC7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //①初始化 GPIO
	
	  GPIO_SetBits(GPIOC,GPIO_Pin_6);//PC6置1
	  GPIO_SetBits(GPIOC,GPIO_Pin_7);//PC7置1
	
	  //两个电机的IO： PB12-15
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
		
		//PB12-15置0
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	  
		//初始化 TIM8
		TIM_TimeBaseStructure.TIM_Period = 899;  //设置在自动重装载周期值
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //设置预分频值
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//设置重复计数器为0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //③初始化 TIMx
		
		//初始化 TIM8 Channe1 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //④初始化外设 TIM8 OC3

		
		//初始化 TIM8 Channe2 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //④初始化外设 TIM8 OC4
		
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能预装载寄存器
		TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); //使能预装载寄存器
		
		TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
		TIM_Cmd(TIM8, ENABLE);  //⑤使能 TIM8
		TIM_CtrlPWMOutputs(TIM8, ENABLE);
		
}

void MOTOR_A(int state)
{
	if(state == GO)//左前电机前进
	{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//PA4置0
	GPIO_SetBits(GPIOC,GPIO_Pin_1);   //PA5置1
	}
	if(state == BACK)//左前电机后退
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);	    //PA4置1
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //PA5置0
	}
		if(state == STOP)//左前电机停止
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);	    //PA4置1
	GPIO_SetBits(GPIOC,GPIO_Pin_1);   //PA5置1
	}
}

//右前B电机
void MOTOR_B(int state)
{
	if(state == GO)//右前电机前进
	{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);	//PA6置0
	GPIO_SetBits(GPIOC,GPIO_Pin_3);   //PA7置1
	}
	if(state == BACK)//右前电机后退
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	    //PA6置1
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);   //PA7置0
	
	}
	if(state == STOP)//右前电机停止
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	    //PA6置1
	GPIO_SetBits(GPIOC,GPIO_Pin_3);   //PA7置1
	}
}

//左后C电机
void MOTOR_C(int state)
{
	if(state == GO)//左后电机前进
	{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//PC6置0
	GPIO_SetBits(GPIOB,GPIO_Pin_13);   //PC7置1

	}
	if(state == BACK)//左后电机后退
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	    //PC6置1
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);   //PC7置0
	
	}
	if(state == STOP)//左后电机停止
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	    //PA6置1
	GPIO_SetBits(GPIOB,GPIO_Pin_13);   //PA7置1
	}
}

//右后D电机
void MOTOR_D(int state)
{
	if(state == GO)//右后电机前进
	{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);	//PC8置0
	GPIO_SetBits(GPIOB,GPIO_Pin_15);   //PC9置1
		
	}
	if(state == BACK)//右后电机后退
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	    //PC8置1
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);   //PC9置0
	}
	if(state == STOP)//左后电机停止
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	    //PC8置1
	GPIO_SetBits(GPIOB,GPIO_Pin_15);   //PC9置1
	}
}
void Car_Go(void)
{
	//左前电机 前    //右前电机 前
	MOTOR_A(GO);       MOTOR_B(GO);

	//左后电机 前   //右后电机 前
	MOTOR_D(GO);       MOTOR_C(GO);
	
	
}

void Car_Stop(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//PA4置0
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);	//PA6置0
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//PC6置0
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);	    //PC8置1
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}

void Car_Back(void)
{
		//左前电机 后    //右前电机 后
	MOTOR_A(BACK);       MOTOR_B(BACK);

	//左后电机 后   //右后电机 后
	MOTOR_D(BACK);       MOTOR_C(BACK);
}

void Car_Turn_Right(void)
{
	
	//左前电机 后    //右前电机 前
	MOTOR_A(BACK);       MOTOR_B(GO);

	//左后电机 后   //右后电机 前
	MOTOR_D(BACK);       MOTOR_C(GO);
	
}

void Car_Turn_Left(void)
{
	//左前电机 前    //右前电机 后
	MOTOR_A(GO);       MOTOR_B(BACK);

	//左后电机 前   //右后电机 后
  MOTOR_D(GO);       MOTOR_C(BACK);
	
}

void Car_Move_Left(void)
{
	//左前电机 前    //右前电机 后
	MOTOR_A(GO);       MOTOR_B(BACK);

	//左后电机 后   //右后电机 前
  MOTOR_D(BACK);       MOTOR_C(GO);
}

void Car_Move_Right(void)
{
	//左前电机 后    //右前电机 前
	MOTOR_A(BACK);       MOTOR_B(GO);

	//左后电机 前   //右后电机 后
  MOTOR_D(GO);       MOTOR_C(BACK);
}

void Car_Turn_lRight(void)
{
	
	//左前电机 后    //右前电机 前
  MOTOR_A(STOP);       MOTOR_B(GO);

	//左后电机 后   //右后电机 前
  MOTOR_D(BACK);       MOTOR_C(STOP);
	
}

void Car_Turn_lLeft(void)
{
	//左前电机 前    //右前电机 后
  MOTOR_A(STOP);       MOTOR_B(BACK);

	//左后电机 前   //右后电机 后
  MOTOR_D(GO);       MOTOR_C(STOP);
	
}
void setcompare(int Lpwm,int Rpwm){
	//前轮
	 TIM_SetCompare3(TIM8,Lpwm);     	  //PB8 修改比较值pwm //左
	 TIM_SetCompare4(TIM8,Rpwm);       	//PB9 修改比较值pwm //右
	//后轮
   TIM_SetCompare1(TIM8,Lpwm);    	  //PB0 修改比较值pwm //左
	 TIM_SetCompare2(TIM8,Rpwm);   	    //PB1 修改比较值pwm //右

}
// pwm control
void pwm_control(int Lpwm,int Rpwm){
	
// set mode and calculate pwm-----------------------------------------
	
	// calculate output
	int output = abs(Lpwm - Rpwm)/2;
	// set delta
	int delta = 14;
	int i=0;
	// Stop
	if ((Lpwm==50)&&(Rpwm==50)){
		  Car_Stop();
      setcompare(100,100);
	}
	else if((Lpwm >= 1000)||(Rpwm >=1000)){
	  Car_Stop();
    setcompare(Lpwm,Rpwm);
		// pwm must be the same
		for (i=0;i<10;i++){
			Lpwm = Rpwm = 400;
			Car_Turn_Right();
			setcompare(Lpwm,Rpwm);
			delay_ms(30);
		}
		Car_Stop();
		setcompare(Lpwm,Rpwm);
	}
	
	// other cases: normal pwm
	else{
	
	  // move forwards: output is in range
		if(output <= delta){
			Car_Go();
			// pwm must be the same
		  Lpwm = Rpwm = (Lpwm + Rpwm)/2;
			setcompare(Lpwm,Rpwm);
		}
	  
		// turn and go: output is not in range
		else{

			// turn left and go forward: left pwm is larger. The speed of left wheels is smaller.
			if(Lpwm > Rpwm){
				Car_Turn_Right();
				 Lpwm = Rpwm = (Lpwm + Rpwm)/2;
			}
			else{
				// turn right and go forward: right pwm is larger. The speed of right wheels is smaller.
			  Car_Turn_Left();
				 Lpwm = Rpwm = (Lpwm + Rpwm)/2;
			}
			setcompare(Lpwm,Rpwm);
		}
	}
}






