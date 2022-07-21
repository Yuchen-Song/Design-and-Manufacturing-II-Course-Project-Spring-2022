#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
				GPIO_InitTypeDef  GPIO_InitStructure;
	
				TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;//TIM��ʱ��������
	
	
        TIM_OCInitTypeDef        TIM_OCInitStructure;
  
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

        //����TIM3��ͨ��1��GPIOA_Pin_6,ͨ��2��GPIOA_Pin_7;ͨ��3��GPIOB_Pin_0��
        //����TIM3��ͨ��4��GPIOB_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        //ʹ��PWM�Ĺ�����Ҫ���ó�AF_PPģʽ
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        //ʹ��TIM3��ʱ����;
        
        //��λ��ʱ��
        TIM_DeInit(TIM3);
    
        //TIM_Prescaler:72,TIM_Period:20000����Ϊ20ms
        TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                        //��ʱ������
        TIM_TimeBaseInitStructure.TIM_Period = arr;                        //��ʱ�����µ���װ��ֵ0-65535
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //��ʼ��TIM3      
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 1000;                                //Ԥ��ֵ
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    
        
        TIM_OC1Init(TIM3,&TIM_OCInitStructure);
        TIM_OC2Init(TIM3,&TIM_OCInitStructure);
        TIM_OC3Init(TIM3,&TIM_OCInitStructure);
        TIM_OC4Init(TIM3,&TIM_OCInitStructure);

        TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
      
        TIM_CtrlPWMOutputs(TIM3, ENABLE);
//        TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
        //ʹ�ܱ�־λCC1,CC2,CC3,CC4
        TIM_Cmd(TIM3,ENABLE);
        //ʹ��TIM;
   
}
