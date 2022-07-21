#ifndef _SOFT_I2C_H__
#define _SOFT_I2C_H__

#include "stm32f10x.h"

#define SCL		!(!(GPIOB->IDR & (1 << 6))) 	// SCL 逻辑状态,PB6
#define SDA		!(!(GPIOB->IDR & (1 << 7)))     // SDA 逻辑状态,PB7

//#define SCL_HIGH()	GPIOB->BSRRL |= 1 << 8 		// SCL 拉高
//#define SCL_LOW()	GPIOB->BSRRH |= 1 << 8 		// SCL 拉低
//#define SDA_HIGH()	GPIOB->BSRRL |= 1 << 9 		// SDA 拉高
//#define SDA_LOW()	GPIOB->BSRRH |= 1 << 9 		// SDA 拉低


#define SCL_Port GPIOB
#define SCL_Pin GPIO_Pin_6

#define SDA_Port GPIOB
#define SDA_Pin GPIO_Pin_7


/******************************************************************************
* 函数介绍： I2C 初始化，
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  配置 GPIO：SCL 推挽输出，SDA 开漏输出
******************************************************************************/
void SOFT_I2C_Init(void);

/******************************************************************************
* 函数介绍： 产生 I2C 协议起始信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Start(void);

/******************************************************************************
* 函数介绍： 产生 I2C 协议结束信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Stop(void);

/******************************************************************************
* 函数介绍： 发送八位数据（不包含应答）
* 输入参数： byte: 要发送的数据
* 输出参数： 无
* 返回值 ：  无
* 备    注： 送数据到 SDA 先后要加延时再拉高时钟，否则时序会出错
******************************************************************************/
void SOFT_I2C_Write_Byte(uint8_t byte);

/******************************************************************************
* 函数介绍： 读取八位数据（不包含应答）
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  byte：返回读到的数据
* 备    注： 使用开漏输出模拟双向口时，读数据前要先拉高
******************************************************************************/
uint8_t SOFT_I2C_Read_Byte(void);

/******************************************************************************
* 函数介绍： 接收应答信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  ack: 接收到的应答信号，1：非应答   0：应答
* 备    注： 使用开漏输出模拟双向口时，读数据前要先拉高
******************************************************************************/
uint8_t SOFT_I2C_Read_ACK(void);

/******************************************************************************
* 函数介绍： 发送应答信号
* 输入参数： ack: 待发送的应答信号，1：非应答   0：应答
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Write_ACK(uint8_t ack);

#endif /* #ifndef _I2C_H__ */
