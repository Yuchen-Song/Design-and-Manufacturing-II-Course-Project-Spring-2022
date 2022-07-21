#include <stdio.h>
#include "mpu6050.h"
#include "i2c.h"

/******************************************************************************
* 函数介绍： MPU6050 初始化函数
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  配置 MPU6050 测量范围：± 2000 °/s  ± 2g
*				  	 如果使用 16 位数据，那么就要 65536 / (2000*2) 将角速度原始读数转换为角速度 °/s
******************************************************************************/
void MPU6050_Init(void)
{
	SOFT_I2C_Init();  // I2C 初始化
	
	MPU6050_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状态
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);        //低通滤波频率，典型值：0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
	MPU6050_Write_Reg(MPU_INT_EN_REG, 0X01);	//开启FIFO中断
	MPU6050_Write_Reg(MPU_INTBP_CFG_REG, 0X80);	//INT引脚0X80低电平触发,0X9c高电平触发
}

/******************************************************************************
* 函数介绍： MPU6050 外部中断初始化函数
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  用 PA5 的外部中断，前面设置低电平触发，所以这里要上拉
******************************************************************************/
void MPU6050_EXTI_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟
	
	/***************** PA 5 配置 **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 // 选择端口 5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // 配置为上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	// 初始化到 A
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);	// 外部中断配置
	
	/***************** 外部中断 配置 **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;	// 外部中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** 中断优先级 配置 **************************/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************
* 函数介绍： MPU6050 写寄存器函数
* 输入参数： regAddr：寄存器地址
             regData：待写入寄存器值
* 输出参数： 无
* 返回值 ：  无
******************************************************************************/
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData)
{
	/* 发送起始信号 */
	SOFT_I2C_Start();
	
	/* 发送设备地址 */		
	SOFT_I2C_Write_Byte(DEV_ADDR);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* 发送寄存器地址 */
	SOFT_I2C_Write_Byte(regAddr);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* 写数据到寄存器 */
	SOFT_I2C_Write_Byte(regData);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
stop:
	SOFT_I2C_Stop();
}

/******************************************************************************
* 函数介绍： MPU6050 读寄存器函数
* 输入参数： regAddr：寄存器地址
* 输出参数： 无
* 返回值 ：  regData：读出的寄存器数据
******************************************************************************/
uint8_t MPU6050_Read_Reg(uint8_t regAddr)
{
	uint8_t regData;
	
	/* 发送起始信号 */
	SOFT_I2C_Start();
	
	/* 发送设备地址 */		
	SOFT_I2C_Write_Byte(DEV_ADDR);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* 发送寄存器地址 */
	SOFT_I2C_Write_Byte(regAddr);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* 发送重复起始信号 */
	SOFT_I2C_Start();
	
	/* 发送读模式设备地址 */		
	SOFT_I2C_Write_Byte(DEV_ADDR | 0x01);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* 读寄存器数据 */
	regData = SOFT_I2C_Read_Byte();
	SOFT_I2C_Write_ACK(1);  // 非应答信号
	
stop:
	SOFT_I2C_Stop();
	
	return regData;
}

/******************************************************************************
* 函数介绍： 连续读两个寄存器并合成 16 位数据
* 输入参数： regAddr：数据低位寄存器地址
* 输出参数： 无
* 返回值 ：  data：2 个寄存器合成的 16 位数据
******************************************************************************/
int16_t MPU6050_Get_Data(uint8_t regAddr)
{
	uint8_t Data_H, Data_L;
	uint16_t data;
	
	Data_H = MPU6050_Read_Reg(regAddr);
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;  // 合成数据
	
	return data;
}

/******************************************************************************
* 函数介绍： 外部中断服务函数
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  无
******************************************************************************/
static float yaw = 0.0f;	// yaw 角
void EXTI9_5_IRQHandler(void)
{
	static int g_GetZeroOffset = 0;	// 计数变量
	static float gyroZ_offset = 0.0f;	// z轴角速度的零偏
	float gyroZ;	// z轴角速度

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) //低电平触发
	{
		EXTI->PR=1<<5; //清楚中断标志位
		
		gyroZ = MPU6050_Get_Data(GYRO_ZOUT_H);	// 陀螺仪的寄存器原始数据
		gyroZ /= 16.384f;	// 65536 / (2000*2)
		
//		// 间隔8ms一次采样，统计125次，共1秒时间
//		if (g_GetZeroOffset++ < ZERO_OFFSET_COUN)	// 如果还没有计算完零偏才会进入这个判断
//		{
//			gyroZ_offset += gyroZ * dt;	// 每次8%积分，累计加权125次
//		}
//		
//		gyroZ = gyroZ- gyroZ_offset-0.1f;	// z轴角速度减去零偏值
		gyroZ += 1.220703;	// 陀螺仪零偏值，目前人工设置
		gyroZ *= 10;
		gyroZ = (float)((int)gyroZ / 10);
		
//		if (g_GetZeroOffset > ZERO_OFFSET_COUN) // 统计完零偏后开始累计偏角，计算完零偏之后才会进入这个判断
//		{
			yaw += gyroZ * dt;	// 每次8%权重累计偏转角度
			// 360°一个循环
			if (yaw > 360)
				yaw -= 360;
			if (yaw < -360)
				yaw += 360;
//		}
		
//		printf("gyroZ_offset: %f\t", gyroZ_offset);
//		printf("GYRO_Z: %f\t", gyroZ);
//		printf("YAW: %f\r\n", yaw);
//		printf("\r\n");
		
		/*这里写相关应用*/
	}
}

/******************************************************************************
* 函数介绍： 读取Yaw
* 输入参数： 无
* 输出参数： 无
* 返回值 ：  无
* 备   注：  无
******************************************************************************/
float MPU6050_Get_Yaw(void)
{
	return yaw;
}
