#include "i2c.h"

// SCL 拉高
void SCL_HIGH(void)
{
	GPIO_SetBits(SCL_Port, SCL_Pin);
}
// SCL 拉低
void SCL_LOW(void)
{
	GPIO_ResetBits(SCL_Port, SCL_Pin);
}
// SDA 拉高
void SDA_HIGH(void)
{
	GPIO_SetBits(SDA_Port, SDA_Pin);
}
// SDA 拉低
void SDA_LOW(void)
{
	GPIO_ResetBits(SDA_Port, SDA_Pin);
}



/******************************************************************************
* 函数介绍： I2C 延时函数
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备    注： 可根据具体情况调整延时值，实现不同速率的传输
******************************************************************************/
void delay(void)  
{
	uint16_t t = 10;
	while (t--);
}

/******************************************************************************
* 函数介绍： I2C 初始化，
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  配置 GPIO：SCL 推挽输出，SDA 开漏输出
******************************************************************************/
void SOFT_I2C_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); // 开启 GPIOB 端口时钟
	
	/***************** PB6 配置 **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 // 选择端口 6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // 配置为推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	// 初始化到 B
	
	/***************** PB7 配置 **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	 // 选择端口 7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 // 配置为开漏输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	// 初始化到 B
	
	/**************** I2C 电平初始化 **********************/
	GPIO_SetBits(GPIOB, GPIO_Pin_6);	// 初始化 PB6 拉高，为 SCL
	GPIO_SetBits(GPIOB, GPIO_Pin_7);	// 初始化 PB7 拉高，为 SDA
}

/******************************************************************************
* 函数介绍： 产生 I2C 协议起始信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Start(void)
{
	SDA_HIGH();
	SCL_HIGH();
	delay();
	SDA_LOW();
	delay();
	SCL_LOW();
	delay();
}

/******************************************************************************
* 函数介绍： 产生 I2C 协议结束信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Stop(void)
{
	SDA_LOW();
	SCL_HIGH();
	delay();
	SDA_HIGH();
	delay();
}

/******************************************************************************
* 函数介绍： 发送八位数据（不包含应答）
* 输入参数： byte: 要发送的数据
* 输出参数： 无
* 返回值 ：  无
* 备    注： 送数据到 SDA 先后要加延时再拉高时钟，否则时序会出错
******************************************************************************/
void SOFT_I2C_Write_Byte(uint8_t byte)
{
	uint16_t i;
	
	SCL_LOW();  // 改变数据线前拉低时钟
	
	for (i = 0; i < 8; i++)
	{
		if (byte & (0x80 >> i))
		{
			SDA_HIGH();
		}
		else
		{
			SDA_LOW();
		}
		delay();		// 必须加延时，等待数据准备好
		
		SCL_HIGH();
		delay();
		SCL_LOW();
		delay();
	}
}

/******************************************************************************
* 函数介绍： 读取八位数据（不包含应答）
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  byte：返回读到的数据
* 备    注： 使用开漏输出模拟双向口时，读数据前要先拉高
******************************************************************************/
uint8_t SOFT_I2C_Read_Byte(void)
{
	uint8_t byte;
	uint16_t i;
	
	for (i = 0; i < 8; i++)
	{
		SDA_HIGH();  // 拉高数据线，读取数据
		SCL_HIGH();
		delay();
		byte <<= 1;
		byte |= SDA;
		SCL_LOW();
		delay();
	}
	
	return byte;
}

/******************************************************************************
* 函数介绍： 接收应答信号
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  ack: 接收到的应答信号，1：非应答   0：应答
* 备    注： 使用开漏输出模拟双向口时，读数据前要先拉高
******************************************************************************/
uint8_t SOFT_I2C_Read_ACK(void)
{
	uint8_t ack;
	
	SDA_HIGH();  // 拉高数据线，读取数据
	SCL_HIGH();
	delay();
	ack = SDA;
	SCL_LOW();
	delay();
	
	return ack;
}

/******************************************************************************
* 函数介绍： 发送应答信号
* 输入参数： ack: 待发送的应答信号，1：非应答   0：应答
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void SOFT_I2C_Write_ACK(uint8_t ack)
{
	SCL_LOW();  // 改变数据线前拉低时钟
	
	if (ack)
	{
		SDA_HIGH();
	}
	else
	{
		SDA_LOW();
	}
	
	SCL_HIGH();
	delay();
	SCL_LOW();
	delay();
}
