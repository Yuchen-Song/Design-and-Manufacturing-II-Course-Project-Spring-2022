#include "sys.h"		    
#include "usart2.h"	 
#include "delay.h"
#include "usart.h"

#define UART2_BUFFER_SIZE 256 
volatile uint8_t rx2Buffer2[UART2_BUFFER_SIZE]; 
volatile uint8_t rxBufferNum2  = 0;
volatile uint8_t h_u32RecCnt2 = 0; 
static volatile uint8_t rx2BufferRTail2  = 0;
u8 str2[8];

int left2=410;
int right2=410;

//≥ı ºªØ
void USART2_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//??GPIOA,D??
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//??USART2?? 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//????2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//????
 
	 
	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;///?????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//????

  USART_Init(USART2, &USART_InitStructure); ; //?????
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //????2??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //?????2?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //????2?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //????????
	NVIC_Init(&NVIC_InitStructure); //??NVIC_InitStruct???????????NVIC???
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
   
  USART_Cmd(USART2, ENABLE);                    
   
}


void USART2_Send_Data(u8 *buf,u8 len)
{
	u8 t; 
  for(t=0;t<len;t++)		
	{		   
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART2,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
}
  
void TR2_Receive(void)
{
	unsigned char BufC; 
	if(uart2Available())
	{
		BufC = uart2Read(); 
		if((BufC==0x41))
		{ 
			BufC = uart2Read(); 
			if((BufC==0x42))
			{
				str2[2] = uart2Read()-0x30; 
				str2[3] = uart2Read()-0x30; 
				str2[4] = uart2Read()-0x30; 
				str2[5] = uart2Read()-0x30; 
				str2[6] = uart2Read()-0x30; 
				str2[7] = uart2Read()-0x30;  
				// get Lpwm and Rpwm
				left2=(signed short int)str2[2]*100+(signed short int)str2[3]*10+(signed short int)str2[4];
				right2=(signed short int)str2[5]*100+(signed short int)str2[6]*10+(signed short int)str2[7];
				// printf("left:%d",left);    
				// printf("right:%d\n",right); 
			} 
		}
	} 
}


/**
* @brief      UartRead
* @param[out]  Ret 1 Byte Data.
* @param[in]  void.
*/
uint8_t uart2Read(void)
{
    uint8_t ch ;
    while  (!h_u32RecCnt2);

    ch = rx2Buffer2[rx2BufferRTail2];
    rx2BufferRTail2++;
    if (rx2BufferRTail2>=UART2_BUFFER_SIZE)
    {
        rx2BufferRTail2  = 0;
    } 
    h_u32RecCnt2--;
    return ch;
}

uint16_t uart2Available(void)
{
    return h_u32RecCnt2;
}

void USART2_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //??????,?????????
	{
		rx2Buffer2[rxBufferNum2] = USART_ReceiveData(USART2); //??8 Bit??
		rxBufferNum2++;
		if (rxBufferNum2 >= UART2_BUFFER_SIZE ) rxBufferNum2 = 0;
		h_u32RecCnt2++;
  } 
} 










