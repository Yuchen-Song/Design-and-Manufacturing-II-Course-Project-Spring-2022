#include "encoder.h"
#include "usart.h"
//void Encode_Motor_Init()//PB12 13
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
//    GPIO_Init(GPIOB,&GPIO_InitStructure);
//}

//void Encode_PWM_Init(u16 arr,u16 psc) //PA8(TIM1 CH1)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructure;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA,ENABLE);

//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA,&GPIO_InitStructure);

//    TIM_TimeBaseStructure.TIM_Period=arr;
//    TIM_TimeBaseStructure.TIM_Prescaler=psc;
//    TIM_TimeBaseStructure.TIM_ClockDivision=0;
//    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

//    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse=0;
//    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
//    TIM_OC1Init(TIM1,&TIM_OCInitStructure);

//    TIM_CtrlPWMOutputs(TIM1,ENABLE);
//    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
//    TIM_ARRPreloadConfig(TIM1,ENABLE);
//    TIM_Cmd(TIM1,ENABLE);

//}

//void Encode_Init()
//{
//    //Encode_Motor_Init();
//    //Encode_PWM_Init(7199,0);
//    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_ICInitTypeDef TIM_ICInitStructure;
//		TIM_OCInitTypeDef TIM_OCInitStructure;
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_TIM1, ENABLE);
//			
//		GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
//	
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
//    GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//推挽输出
//    GPIO_Init(GPIOB,&GPIO_InitStructure);

//    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//    TIM_TimeBaseStructure.TIM_Prescaler=0x0;
//		
//    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD;
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

//    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3

//    TIM_ICStructInit(&TIM_ICInitStructure);
//    TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;                  
//		TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	      
//		TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   
//		TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	            
//		TIM_ICInitStructure.TIM_ICFilter=10; 
//    TIM_ICInit(TIM4,&TIM_ICInitStructure);
//		
//		TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;                  
//		TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	      
//		TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   
//		TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	            
//		TIM_ICInitStructure.TIM_ICFilter=10; 
//    TIM_ICInit(TIM4,&TIM_ICInitStructure);
//		
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ PWM Ä£Ê½ 2
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Êä³ö¼«ÐÔ¸ß
//		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //¢Ü³õÊ¼»¯ÍâÉè TIM4 OC1
//		
//		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ PWM Ä£Ê½ 2
//		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
//		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //Êä³ö¼«ÐÔ¸ß
//		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //¢Ü³õÊ¼»¯ÍâÉè TIM4 OC1
//		
//		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //Ê¹ÄÜÔ¤×°ÔØ¼Ä´æÆ÷
//		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //Ê¹ÄÜÔ¤×°ÔØ¼Ä´æÆ÷
//		
//		TIM_ARRPreloadConfig(TIM4, ENABLE); //Ê¹ÄÜTIMxÔÚARRÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
//		
//    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
//    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

//    TIM_SetCounter(TIM4,0);
//    TIM_Cmd(TIM4,ENABLE);
//}


static int flag = 0;
static pd pd_s_11, pd_s_12, pd_s_13, pd_s_14;

/*
 *@brief：编码器初始化
 */
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/***************** 使能时钟 **************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	// GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO即外部中断时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	// TIM2
	
	/***************** PC 11|12 配置 **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;	 // 端口 11|12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // 上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);	// 外部中断配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);	// 外部中断配置
	/***************** PA 13|14 配置 **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	 // 端口 13|14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // 上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);	// 外部中断配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);	// 外部中断配置
	
	/***************** 外部中断线11 配置 **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line11|EXTI_Line12|EXTI_Line13|EXTI_Line14;	// Íâ²¿ÖÐ¶Ï
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØ´¥·¢
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** 外部中断线12 配置 **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;	// Íâ²¿ÖÐ¶Ï
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØ´¥·¢
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** 外部中断线13 配置 **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;	// Íâ²¿ÖÐ¶Ï
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØ´¥·¢
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** 外部中断线14 配置 **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	// Íâ²¿ÖÐ¶Ï
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØ´¥·¢
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** 外部中断优先级 配置 **************************/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //ÇÀÕ¼ÓÅÏÈ¼¶2£¬ 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	/***************** 定时器TIM2 配置 **************************/
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseStructure.TIM_Period = 100 * 10; // 100ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	/***************** 定时器TIM2 中断优先级 配置 **************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //ÇÀÕ¼ÓÅÏÈ¼¶2£¬ 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 允许定时器更新中断
	
  TIM_Cmd(TIM2, ENABLE); 	// 使能TIM2
	
	PD_Init();//初始化PD控制器
}

static u32 counter_11 = 0;
static u32 counter_12 = 0;
static u32 counter_13 = 0;
static u32 counter_14 = 0;
static float speed_11, speed_12,speed_13,speed_14;
static u32 set_11, set_12, set_13,set_14;

/*
 *@brief：外部中断函数，counter加一
 *@param：none
 *@retval:none
 */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		counter_11 = counter_11 + 1;
		//printf("counter: %d\r\n\n",counter_11);
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		counter_12 ++;
		//printf("%d\n",counter);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		counter_13 ++;
		//printf("%d\n",counter);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		counter_14 ++;
		//printf("%d\n",counter);
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
 
/*
 *@brief：TIM2中断函数，打印counter，置零counter
 */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		speed_11 = (float)counter_11*10.0;
		//speed_11 = (float)counter_11*2/11;
		//printf("Pulse per Second:%d \r\n\n", (int)speed_11);
		counter_11 = 0;
		speed_12 = (float)counter_12*10.0;
		//speed_11 = (float)counter_11*2/11;
		//printf("counter12:%d \r\n\n", counter_12);
		counter_12 = 0;
		speed_13 = (float)counter_13*10.0;
		//speed_11 = (float)counter_11*2/11;
		//printf("counter13:%d \r\n\n", counter_13);
		counter_13 = 0;
		speed_14 = (float)counter_14*10.0;
		//speed_11 = (float)counter_11*2/11;
		//printf("counter14:%d \r\n\n", counter_14);
		counter_14 = 0;
		//printf("speed11:%.2f, ", speed_11);
		
		//当flag = 1时，需要反馈调速
		if (flag == 1)
		{
			float pd_11, pd_12,pd_13,pd_14;
			int pps_11, pps_12,pps_13,pps_14;
			pps_11 = Read_Encoder(11);
			pd_11 = pd_cal(pps_11,&pd_s_11);
			printf("%d\r\n",(int)speed_11);
			//printf("Pulse per Second:%d \r\n",(int)speed_11);
			pd_s_11.PWM = (int)pd_11 + pd_s_11.PWM;
			if (pd_s_11.PWM> 899) pd_s_11.PWM = 899;
			if (pd_s_11.PWM< 0) pd_s_11.PWM = 0;
			
			pps_12 = Read_Encoder(12);
			pd_12 = pd_cal(pps_12,&pd_s_12);
			pd_s_12.PWM = (int)pd_12 + pd_s_12.PWM;
			if (pd_s_12.PWM> 899) pd_s_12.PWM = 899;
			if (pd_s_12.PWM< 0) pd_s_12.PWM = 0;
			
			pps_13 = Read_Encoder(13);
			pd_13 = pd_cal(pps_13,&pd_s_13);
			pd_s_13.PWM = (int)pd_13 + pd_s_13.PWM;
			if (pd_s_13.PWM> 899) pd_s_13.PWM = 899;
			if (pd_s_13.PWM< 0) pd_s_13.PWM = 0;
			
			pps_14 = Read_Encoder(14);
			pd_14 = pd_cal(pps_11,&pd_s_11);
			pd_s_14.PWM = (int)pd_14 + pd_s_14.PWM;
			if (pd_s_14.PWM> 899) pd_s_14.PWM = 899;
			if (pd_s_14.PWM< 0) pd_s_14.PWM = 0;
			
			TIM_SetCompare3(TIM8,pd_s_11.PWM);
			TIM_SetCompare4(TIM8,pd_s_12.PWM);
			TIM_SetCompare1(TIM8,pd_s_13.PWM);
			TIM_SetCompare2(TIM8,pd_s_14.PWM);
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

float Read_Encoder(int a)
{
//    int Encoder_TIM;    
//        Encoder_TIM= (short)TIM2 -> CNT;  
//        TIM2 -> CNT=0; 
//        return Encoder_TIM;
    float Encoder = 0.0;  
		if (a == 11) Encoder= speed_11;  
		else if (a == 12) Encoder= speed_12;  
    else if (a == 13) Encoder= speed_13; 
		else if (a == 14) Encoder= speed_14; 			
    return Encoder;
}

void Set_encoder_flag(int a)
{
	if (a == 0 || a== 1)
		flag = a;
}

void pd_init(pd *p)
{
	p->Kp = 0.15 ;
	p->Kd = 0.2 ;
	p->set  = 0.0;
	p->ekk = 0;
	p->PWM = 699;
}

void PD_Init(void)
{
	pd_init(&pd_s_11);
	pd_init(&pd_s_12);
	pd_init(&pd_s_13);
	pd_init(&pd_s_14);
}

float pd_cal(int pps,  pd *p)
{	
	float ek=0,pd_value=0;
	ek = pps-p->set ;
	pd_value = p->Kp*ek + p->Kd*(ek-p->ekk);
	p->ekk = ek;
	return pd_value;
}

void Set_speed_11(int set)
{
	pd_s_11.set = set;
}

void Set_speed_12(int set)
{
	pd_s_12.set = set;
}

void Set_speed_13(int set)
{
	pd_s_13.set = set;
}

void Set_speed_14(int set)
{
	pd_s_14.set = set;
}