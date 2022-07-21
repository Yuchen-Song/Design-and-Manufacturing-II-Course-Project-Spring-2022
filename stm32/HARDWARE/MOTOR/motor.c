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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //��ʹ�ܶ�ʱ�� 8 ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM��� PC8/PC9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	  GPIO_SetBits(GPIOC,GPIO_Pin_8);//PC8��1
	  GPIO_SetBits(GPIOC,GPIO_Pin_9);//PC9��1
	
	  //���������IO�� PC0-3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
		
		//PC 0-3��0
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	  
		//��ʼ�� TIM8
		TIM_TimeBaseStructure.TIM_Period = 899;  //�������Զ���װ������ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�����ظ�������Ϊ0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
		
		//��ʼ�� TIM8 Channe3 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM8 OC3

		
		//��ʼ�� TIM8 Channe4 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM8 OC4
		
		TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM8, ENABLE);  //��ʹ�� TIM8
		TIM_CtrlPWMOutputs(TIM8, ENABLE); // TIM8����ͨ��ʱ����֮ͬ��
		
}
void B_MOTOR_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //��ʹ�ܶ�ʱ�� 3 ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM��� PC6/PC7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	  GPIO_SetBits(GPIOC,GPIO_Pin_6);//PC6��1
	  GPIO_SetBits(GPIOC,GPIO_Pin_7);//PC7��1
	
	  //���������IO�� PB12-15
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
		
		//PB12-15��0
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	  
		//��ʼ�� TIM8
		TIM_TimeBaseStructure.TIM_Period = 899;  //�������Զ���װ������ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�����ظ�������Ϊ0
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
		
		//��ʼ�� TIM8 Channe1 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM8 OC3

		
		//��ʼ�� TIM8 Channe2 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		TIM_OCInitStructure.TIM_Pulse=0;
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM8 OC4
		
		TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM8, ENABLE);  //��ʹ�� TIM8
		TIM_CtrlPWMOutputs(TIM8, ENABLE);
		
}

void MOTOR_A(int state)
{
	if(state == GO)//��ǰ���ǰ��
	{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//PA4��0
	GPIO_SetBits(GPIOC,GPIO_Pin_1);   //PA5��1
	}
	if(state == BACK)//��ǰ�������
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);	    //PA4��1
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //PA5��0
	}
		if(state == STOP)//��ǰ���ֹͣ
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);	    //PA4��1
	GPIO_SetBits(GPIOC,GPIO_Pin_1);   //PA5��1
	}
}

//��ǰB���
void MOTOR_B(int state)
{
	if(state == GO)//��ǰ���ǰ��
	{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);	//PA6��0
	GPIO_SetBits(GPIOC,GPIO_Pin_3);   //PA7��1
	}
	if(state == BACK)//��ǰ�������
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	    //PA6��1
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);   //PA7��0
	
	}
	if(state == STOP)//��ǰ���ֹͣ
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	    //PA6��1
	GPIO_SetBits(GPIOC,GPIO_Pin_3);   //PA7��1
	}
}

//���C���
void MOTOR_C(int state)
{
	if(state == GO)//�����ǰ��
	{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//PC6��0
	GPIO_SetBits(GPIOB,GPIO_Pin_13);   //PC7��1

	}
	if(state == BACK)//���������
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	    //PC6��1
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);   //PC7��0
	
	}
	if(state == STOP)//�����ֹͣ
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);	    //PA6��1
	GPIO_SetBits(GPIOB,GPIO_Pin_13);   //PA7��1
	}
}

//�Һ�D���
void MOTOR_D(int state)
{
	if(state == GO)//�Һ���ǰ��
	{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);	//PC8��0
	GPIO_SetBits(GPIOB,GPIO_Pin_15);   //PC9��1
		
	}
	if(state == BACK)//�Һ�������
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	    //PC8��1
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);   //PC9��0
	}
	if(state == STOP)//�����ֹͣ
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);	    //PC8��1
	GPIO_SetBits(GPIOB,GPIO_Pin_15);   //PC9��1
	}
}
void Car_Go(void)
{
	//��ǰ��� ǰ    //��ǰ��� ǰ
	MOTOR_A(GO);       MOTOR_B(GO);

	//����� ǰ   //�Һ��� ǰ
	MOTOR_D(GO);       MOTOR_C(GO);
	
	
}

void Car_Stop(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	//PA4��0
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);	//PA6��0
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//PC6��0
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);	    //PC8��1
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}

void Car_Back(void)
{
		//��ǰ��� ��    //��ǰ��� ��
	MOTOR_A(BACK);       MOTOR_B(BACK);

	//����� ��   //�Һ��� ��
	MOTOR_D(BACK);       MOTOR_C(BACK);
}

void Car_Turn_Right(void)
{
	
	//��ǰ��� ��    //��ǰ��� ǰ
	MOTOR_A(BACK);       MOTOR_B(GO);

	//����� ��   //�Һ��� ǰ
	MOTOR_D(BACK);       MOTOR_C(GO);
	
}

void Car_Turn_Left(void)
{
	//��ǰ��� ǰ    //��ǰ��� ��
	MOTOR_A(GO);       MOTOR_B(BACK);

	//����� ǰ   //�Һ��� ��
  MOTOR_D(GO);       MOTOR_C(BACK);
	
}

void Car_Move_Left(void)
{
	//��ǰ��� ǰ    //��ǰ��� ��
	MOTOR_A(GO);       MOTOR_B(BACK);

	//����� ��   //�Һ��� ǰ
  MOTOR_D(BACK);       MOTOR_C(GO);
}

void Car_Move_Right(void)
{
	//��ǰ��� ��    //��ǰ��� ǰ
	MOTOR_A(BACK);       MOTOR_B(GO);

	//����� ǰ   //�Һ��� ��
  MOTOR_D(GO);       MOTOR_C(BACK);
}

void Car_Turn_lRight(void)
{
	
	//��ǰ��� ��    //��ǰ��� ǰ
  MOTOR_A(STOP);       MOTOR_B(GO);

	//����� ��   //�Һ��� ǰ
  MOTOR_D(BACK);       MOTOR_C(STOP);
	
}

void Car_Turn_lLeft(void)
{
	//��ǰ��� ǰ    //��ǰ��� ��
  MOTOR_A(STOP);       MOTOR_B(BACK);

	//����� ǰ   //�Һ��� ��
  MOTOR_D(GO);       MOTOR_C(STOP);
	
}
void setcompare(int Lpwm,int Rpwm){
	//ǰ��
	 TIM_SetCompare3(TIM8,Lpwm);     	  //PB8 �޸ıȽ�ֵpwm //��
	 TIM_SetCompare4(TIM8,Rpwm);       	//PB9 �޸ıȽ�ֵpwm //��
	//����
   TIM_SetCompare1(TIM8,Lpwm);    	  //PB0 �޸ıȽ�ֵpwm //��
	 TIM_SetCompare2(TIM8,Rpwm);   	    //PB1 �޸ıȽ�ֵpwm //��

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






