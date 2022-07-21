#include "sonic.h"
#include "usart.h"
#include "delay.h"

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  //PA1 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA1 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA1 ����
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//���������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM5,ENABLE); 	//ʹ�ܶ�ʱ��5
}

u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
u8  TIM5CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			//printf("Captured!/n");
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				//printf("Captured Down!/n");
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				//printf("Captured Up!/n");
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}	
			
 	}
	if((TIM5CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH2_CAPTURE_VAL=0XFFFF;
				}else TIM5CH2_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//����1���������¼�
		{	
			//printf("Captured!/n");
			if(TIM5CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				//printf("Captured Down!/n");
				TIM5CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);
		   		TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				//printf("Captured Up!/n");
				TIM5CH2_CAPTURE_STA=0;			//���
				TIM5CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}
	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
 
}


void TIM4_Cap_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM4_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //PB8 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB8 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //PB8 ����
	
	
	//��ʼ����ʱ��4 TIM4 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM4���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);//���������ж� ,���������жϺͲ����ж�	
	
  TIM_Cmd(TIM4,ENABLE); 	//ʹ��TIM4
}

u8  TIM4CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM4CH3_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��5�жϷ������	 
void TIM4_IRQHandler(void)
{
 	if((TIM4CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM4CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM4CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM4CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM4CH3_CAPTURE_VAL=0XFFFF;
				}else TIM4CH3_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//����1���������¼�
		{	
			//printf("Captured!/n");
			if(TIM4CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				//printf("Captured Down!/n");
				TIM4CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM4CH3_CAPTURE_VAL=TIM_GetCapture3(TIM4);
				TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				//printf("Captured Up!/n");
				TIM4CH3_CAPTURE_STA=0;			//���
				TIM4CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM4,0);
				TIM4CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}	
 	}
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC3|TIM_IT_Update); //����жϱ�־λ
}

void HC_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //??PA????
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // ??????(Trig)??PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // ??????(Trig)??PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // ??????(Trig)??PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
}

float HC_SR04_GetDis(u8 a)
{
	float Distance = 0.0;
	u32 temp=0; 
	if (a == 1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);    //TRIG??
		delay_us(20);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);  //TRIG??

		if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			Distance = temp*0.017;
			printf("HIGH-1:%.2f cm\r\n",Distance);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
			return Distance;
		}
	}
	else if (a == 3)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_4);    //TRIG??
		delay_us(20);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);  //TRIG??

		if(TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ��������
			{
				temp=TIM5CH2_CAPTURE_STA&0X3F;
				temp*=65536;//���ʱ���ܺ�
				temp+=TIM5CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				Distance = temp*0.017;
				printf("HIGH-2:%.2f cm\r\n",Distance);//��ӡ�ܵĸߵ�ƽʱ��
				TIM5CH2_CAPTURE_STA=0;//������һ�β���
				return Distance;
		}
	}
	else if (a == 2)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_5);    //TRIG??
		delay_us(20);
		GPIO_ResetBits(GPIOC,GPIO_Pin_5);  //TRIG??

		if(TIM4CH3_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM4CH3_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM4CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			Distance = temp*0.017;
			printf("HIGH-3:%.2f cm\r\n",Distance);//��ӡ�ܵĸߵ�ƽʱ��
			TIM4CH3_CAPTURE_STA=0;//������һ�β���
			return Distance;
		}
	}
}



