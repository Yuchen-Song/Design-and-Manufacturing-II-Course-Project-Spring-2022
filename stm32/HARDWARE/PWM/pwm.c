#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
				GPIO_InitTypeDef  GPIO_InitStructure;
	
				TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;//TIM定时器的配置
	
	
        TIM_OCInitTypeDef        TIM_OCInitStructure;
  
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

        //这里TIM3的通道1是GPIOA_Pin_6,通道2是GPIOA_Pin_7;通道3是GPIOB_Pin_0；
        //这里TIM3的通道4是GPIOB_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        //使用PWM的功能需要设置成AF_PP模式
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        //使能TIM3的时钟线;
        
        //复位定时器
        TIM_DeInit(TIM3);
    
        //TIM_Prescaler:72,TIM_Period:20000周期为20ms
        TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                        //定时器周期
        TIM_TimeBaseInitStructure.TIM_Period = arr;                        //定时器更新的重装载值0-65535
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //初始化TIM3      
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 1000;                                //预设值
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
        //使能标志位CC1,CC2,CC3,CC4
        TIM_Cmd(TIM3,ENABLE);
        //使能TIM;
   
}
