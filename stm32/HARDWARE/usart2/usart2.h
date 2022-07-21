#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


#define USART_REC_LEN  			200  	
#define EN_USART2_RX 			1		
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 
extern u16 USART_RX_STA;         		

void USART2_Init(u32 bound);
void USART2_Send_Data(u8 *buf,u8 len);
void TR2_Receive(void);
uint8_t uart2Read(void);
uint16_t uart2Available(void);
void USART2_IRQHandler(void);

#endif


