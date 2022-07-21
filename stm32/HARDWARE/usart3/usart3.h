#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


#define USART_REC_LEN  			200  	
#define EN_USART3_RX 			1		
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 
extern u16 USART_RX_STA;         		

void USART3_Init(u32 bound);
void USART3_Send_Data(u8 *buf,u8 len);
void TR3_Receive(void);
uint8_t uart3Read(void);
uint16_t uart3Available(void);
void USART3_IRQHandler(void);

#endif
