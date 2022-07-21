#include "sys.h"		    
#include "usart3.h"	 
#include "delay.h"

#define UART3_BUFFER_SIZE 256 
volatile uint8_t rx2Buffer1[UART3_BUFFER_SIZE]; 
volatile uint8_t rxBufferNum1  = 0;
volatile uint8_t h_u32RecCnt1 = 0; 
static volatile uint8_t rx2BufferRTail  = 0;
u8 str1[8];

int left1=0;
int right1=0;

//≥ı ºªØ
void USART3_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//??GPIOA,D??
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//??USART1?? 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PA9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//????
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//????2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//????
 
	 
	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;///?????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//????

  USART_Init(USART3, &USART_InitStructure); ; //?????
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //????2??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //?????2?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //????2?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //????????
	NVIC_Init(&NVIC_InitStructure); //??NVIC_InitStruct???????????NVIC???
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   
  USART_Cmd(USART3, ENABLE);                    
   
}


void USART3_Send_Data(u8 *buf,u8 len)
{
	u8 t; 
  for(t=0;t<len;t++)		
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
}
  



/**
* @brief      UartRead
* @param[out]  Ret 1 Byte Data.
* @param[in]  void.
*/
uint8_t uart3Read(void)
{
    uint8_t ch ;
    while  (!h_u32RecCnt1);

    ch = rx2Buffer1[rx2BufferRTail];
    rx2BufferRTail++;
    if (rx2BufferRTail>=UART3_BUFFER_SIZE)
    {
        rx2BufferRTail  = 0;
    } 
    h_u32RecCnt1--;
    return ch;
}

uint16_t uart3Available(void)
{
    return h_u32RecCnt1;
}

void USART3_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //??????,?????????
	{
		rx2Buffer1[rxBufferNum1] = USART_ReceiveData(USART3); //??8 Bit??
		rxBufferNum1++;
		if (rxBufferNum1 >= UART3_BUFFER_SIZE ) rxBufferNum1 = 0;
		h_u32RecCnt1++;
  } 
} 










