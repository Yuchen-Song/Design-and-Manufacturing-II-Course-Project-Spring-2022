#include <stdio.h>
#include "mpu6050.h"
#include "i2c.h"

/******************************************************************************
* �������ܣ� MPU6050 ��ʼ������
* ��������� ��
* ��������� ��
* ����ֵ ��  ��
* ��   ע��  ���� MPU6050 ������Χ���� 2000 ��/s  �� 2g
*				  	 ���ʹ�� 16 λ���ݣ���ô��Ҫ 65536 / (2000*2) �����ٶ�ԭʼ����ת��Ϊ���ٶ� ��/s
******************************************************************************/
void MPU6050_Init(void)
{
	SOFT_I2C_Init();  // I2C ��ʼ��
	
	MPU6050_Write_Reg(PWR_MGMT_1, 0x00);	//�������״̬
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //�����ǲ����ʣ�����ֵ��0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);        //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
	MPU6050_Write_Reg(MPU_INT_EN_REG, 0X01);	//����FIFO�ж�
	MPU6050_Write_Reg(MPU_INTBP_CFG_REG, 0X80);	//INT����0X80�͵�ƽ����,0X9c�ߵ�ƽ����
}

/******************************************************************************
* �������ܣ� MPU6050 �ⲿ�жϳ�ʼ������
* ��������� ��
* ��������� ��
* ����ֵ ��  ��
* ��   ע��  �� PA5 ���ⲿ�жϣ�ǰ�����õ͵�ƽ��������������Ҫ����
******************************************************************************/
void MPU6050_EXTI_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PA�˿�ʱ��
	
	/***************** PA 5 ���� **************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 // ѡ��˿� 5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // ����Ϊ��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	// ��ʼ���� A
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);	// �ⲿ�ж�����
	
	/***************** �ⲿ�ж� ���� **************************/
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;	// �ⲿ�ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/***************** �ж����ȼ� ���� **************************/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************
* �������ܣ� MPU6050 д�Ĵ�������
* ��������� regAddr���Ĵ�����ַ
             regData����д��Ĵ���ֵ
* ��������� ��
* ����ֵ ��  ��
******************************************************************************/
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData)
{
	/* ������ʼ�ź� */
	SOFT_I2C_Start();
	
	/* �����豸��ַ */		
	SOFT_I2C_Write_Byte(DEV_ADDR);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* ���ͼĴ�����ַ */
	SOFT_I2C_Write_Byte(regAddr);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* д���ݵ��Ĵ��� */
	SOFT_I2C_Write_Byte(regData);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
stop:
	SOFT_I2C_Stop();
}

/******************************************************************************
* �������ܣ� MPU6050 ���Ĵ�������
* ��������� regAddr���Ĵ�����ַ
* ��������� ��
* ����ֵ ��  regData�������ļĴ�������
******************************************************************************/
uint8_t MPU6050_Read_Reg(uint8_t regAddr)
{
	uint8_t regData;
	
	/* ������ʼ�ź� */
	SOFT_I2C_Start();
	
	/* �����豸��ַ */		
	SOFT_I2C_Write_Byte(DEV_ADDR);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* ���ͼĴ�����ַ */
	SOFT_I2C_Write_Byte(regAddr);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* �����ظ���ʼ�ź� */
	SOFT_I2C_Start();
	
	/* ���Ͷ�ģʽ�豸��ַ */		
	SOFT_I2C_Write_Byte(DEV_ADDR | 0x01);
	if (SOFT_I2C_Read_ACK())
		goto stop;
	
	/* ���Ĵ������� */
	regData = SOFT_I2C_Read_Byte();
	SOFT_I2C_Write_ACK(1);  // ��Ӧ���ź�
	
stop:
	SOFT_I2C_Stop();
	
	return regData;
}

/******************************************************************************
* �������ܣ� �����������Ĵ������ϳ� 16 λ����
* ��������� regAddr�����ݵ�λ�Ĵ�����ַ
* ��������� ��
* ����ֵ ��  data��2 ���Ĵ����ϳɵ� 16 λ����
******************************************************************************/
int16_t MPU6050_Get_Data(uint8_t regAddr)
{
	uint8_t Data_H, Data_L;
	uint16_t data;
	
	Data_H = MPU6050_Read_Reg(regAddr);
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;  // �ϳ�����
	
	return data;
}

/******************************************************************************
* �������ܣ� �ⲿ�жϷ�����
* ��������� ��
* ��������� ��
* ����ֵ ��  ��
* ��   ע��  ��
******************************************************************************/
static float yaw = 0.0f;	// yaw ��
void EXTI9_5_IRQHandler(void)
{
	static int g_GetZeroOffset = 0;	// ��������
	static float gyroZ_offset = 0.0f;	// z����ٶȵ���ƫ
	float gyroZ;	// z����ٶ�

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) //�͵�ƽ����
	{
		EXTI->PR=1<<5; //����жϱ�־λ
		
		gyroZ = MPU6050_Get_Data(GYRO_ZOUT_H);	// �����ǵļĴ���ԭʼ����
		gyroZ /= 16.384f;	// 65536 / (2000*2)
		
//		// ���8msһ�β�����ͳ��125�Σ���1��ʱ��
//		if (g_GetZeroOffset++ < ZERO_OFFSET_COUN)	// �����û�м�������ƫ�Ż��������ж�
//		{
//			gyroZ_offset += gyroZ * dt;	// ÿ��8%���֣��ۼƼ�Ȩ125��
//		}
//		
//		gyroZ = gyroZ- gyroZ_offset-0.1f;	// z����ٶȼ�ȥ��ƫֵ
		gyroZ += 1.220703;	// ��������ƫֵ��Ŀǰ�˹�����
		gyroZ *= 10;
		gyroZ = (float)((int)gyroZ / 10);
		
//		if (g_GetZeroOffset > ZERO_OFFSET_COUN) // ͳ������ƫ��ʼ�ۼ�ƫ�ǣ���������ƫ֮��Ż��������ж�
//		{
			yaw += gyroZ * dt;	// ÿ��8%Ȩ���ۼ�ƫת�Ƕ�
			// 360��һ��ѭ��
			if (yaw > 360)
				yaw -= 360;
			if (yaw < -360)
				yaw += 360;
//		}
		
//		printf("gyroZ_offset: %f\t", gyroZ_offset);
//		printf("GYRO_Z: %f\t", gyroZ);
//		printf("YAW: %f\r\n", yaw);
//		printf("\r\n");
		
		/*����д���Ӧ��*/
	}
}

/******************************************************************************
* �������ܣ� ��ȡYaw
* ��������� ��
* ��������� ��
* ����ֵ ��  ��
* ��   ע��  ��
******************************************************************************/
float MPU6050_Get_Yaw(void)
{
	return yaw;
}
