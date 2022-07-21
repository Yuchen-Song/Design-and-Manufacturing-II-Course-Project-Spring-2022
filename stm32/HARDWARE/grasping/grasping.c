#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart3.h"
#include "pwm.h"
#include "math.h"
#include "grasping.h"

signed short int curr_x;
signed short int curr_y;
signed short int curr_z;
u8 i;//dummy variable

 void fkine(u16 p1, u16 p2, u16 p3, u16 p4){ // forward kinematic (for calibration)
	TIM_SetCompare1(TIM3, p1 + 45);
	TIM_SetCompare2(TIM3, p2 + 45);	   
	TIM_SetCompare3(TIM3, p3 + 45);	  
	TIM_SetCompare4(TIM3, p4 + 45);	  
 }


 void ikine(signed short int x, signed short int y, signed short int z, signed short int gripper){ // inverse kinematic
	double l0 = 0;
	double l1 = 135;
	double l2 = 147;
	double l3 = 88;
	double d0 = 198;
	double d3 = 100;
	double d4 = 0;
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double h;
	double theta_1;
	double theta_2;
	double theta_3;
	
	a = sqrt(x * x + y * y) - l0 - l3;
	b = z - d0 + d3 + d4;
	c = 2 * a * l1;
	d = 2 * b * l1;
	e = 2 * a * l2;
	f = 2 * b * l2;
	g = - a * a - b * b - l1 * l1 + l2 * l2;
	h = - a * a - b * b + l1 * l1 - l2 * l2;
			
	theta_1 = atan((y + 0.000) / (x + 0.000));
	theta_2 = 2 * atan((d + sqrt(c * c + d * d - g * g)) / (c - g));
	theta_3 = 2 * atan((f - sqrt(f * f + e * e - h * h)) / (e - h)) - theta_2;
	
	
	TIM_SetCompare1(TIM3, 45 + (signed short int)(theta_1 * 180 / 3.14159265359) + 90);
	TIM_SetCompare2(TIM3, 225 - (signed short int)(theta_2 * 180 / 3.14159265359 - 12));
	TIM_SetCompare3(TIM3, 135 + (signed short int)((theta_3 + theta_2) * 180 / 3.14159265359 ));
	TIM_SetCompare4(TIM3, gripper+45);
	
	curr_x = x;
	curr_y = y;
  curr_z = z;
 }
 

 void initPose(signed short int tar_g){ // back to initial pose
	 ikine(170, 0, 200, tar_g);
 }
 
 void toPose(signed short int tar_x, signed short int tar_y, signed short int tar_z, signed short int tar_g){
	 signed short int index;
	 signed short int step_num = 20;
	 signed short int start_x = curr_x;
   signed short int start_y = curr_y;
	 signed short int start_z = curr_z;
	 double x_step = (double)((tar_x - start_x)/step_num);
	 double y_step = (double)((tar_y - start_y)/step_num);
	 double z_step = (double)((tar_z - start_z)/step_num);
	 double x_c = (double)start_x;
	 double y_c = (double)start_y;
	 double z_c = (double)start_z;
	 
	 for(index = 1; index <= step_num; index ++){
		ikine((signed short int)(x_c + index * x_step), (signed short int)(y_c + index * y_step), (signed short int)(z_c + index * z_step), tar_g);
		delay_ms(50);
	 }
	 
}


 void fromZ(signed short int _x, signed short int _y, signed short int _z){
	toPose(_x, _y, curr_z, 225);
	toPose(_x, _y, _z, 225);
 }
 
 

 void gripperCmd(signed short int val){
	TIM_SetCompare4(TIM3, val+45);
 }
 
int graspingStage(int Flag)
{
	u8 str1[14];
	 
	unsigned char BufC; 
	 
	signed short int x_axis = 0;
	signed short int y_axis = 0;
	signed short int z_axis = 0;
	signed short int gripperVal = 0;

	if(Flag == 1)
	{
		while(1)
		{
			printf("enter\r\n");
			if(uart3Available())
			{
				BufC = uart3Read(); 
				
				if(BufC == 0x41)
				{ 
					printf("enter A\r\n");
					BufC = uart3Read(); 
					if(BufC == 0x43)
					{
						return 1;//stop
					}
					if(BufC == 0x42)
					{
						for (i=0;i<12;i++)
						{
							str1[i+2] = uart3Read() - 0x30;
						}
						printf("grasping \r\n");
						x_axis = str1[2] * 100 + str1[3] * 10 + str1[4];
						y_axis = str1[5] * 100 + str1[6] * 10 + str1[7];
						z_axis = str1[8] * 100 + str1[9] * 10 + str1[10];
						gripperVal = str1[11] * 100 + str1[12] * 10 + str1[13];
						
						y_axis = y_axis - 300;
						
						initPose(180);
						delay_ms(1000);
						toPose(x_axis, y_axis, curr_z, 180);
						toPose(x_axis, y_axis, z_axis, 180);
						gripperCmd(gripperVal);
						toPose(170, 0, curr_z, gripperVal); // orange final pose
						toPose(170, 0, 200, gripperVal);
						delay_ms(1000);
					
						if (gripperVal == 80)
						{
							toPose(curr_x, curr_y, curr_z+30, gripperVal);
							toPose(1, 165, curr_z, gripperVal); // eraser final pose
							toPose(1, 165, 170+30, gripperVal);
						}
						if (gripperVal == 130)
						{
							toPose(20, 165, curr_z, gripperVal); // orange final pose
							toPose(20, 165, 170, gripperVal);
						}
						delay_ms(1000);
						gripperCmd(180);
						return 2;//finished grasping
					}
				}
			}
		}
	}
	 
	while(1)
	{
	//		if(!uart3Available()) {
	//			return 0;
	//		}
		if(uart3Available())
		{
			BufC = uart3Read(); 
			printf("%x\r\n",BufC);
			if(BufC == 0x41)
			{ 
				BufC = uart3Read(); 
				if(BufC == 0x43)
				{
					return 1;//stop
				}
				if(BufC == 0x42)
				{
					for (i=0;i<12;i++)
					{
						str1[i+2] = uart3Read() - 0x30;
					}
					printf("grasping \r\n");
					x_axis = str1[2] * 100 + str1[3] * 10 + str1[4];
					y_axis = str1[5] * 100 + str1[6] * 10 + str1[7];
					z_axis = str1[8] * 100 + str1[9] * 10 + str1[10];
					gripperVal = str1[11] * 100 + str1[12] * 10 + str1[13];
					
					y_axis = y_axis - 300;
					
					initPose(180);
					delay_ms(1000);
					toPose(x_axis, y_axis, curr_z, 180);
					toPose(x_axis, y_axis, z_axis, 180);
					gripperCmd(gripperVal);
					toPose(170, 0, curr_z, gripperVal); // orange final pose
					toPose(170, 0, 200, gripperVal);
					delay_ms(1000);
					
					if (gripperVal == 80)
					{
						toPose(curr_x, curr_y, curr_z+30, gripperVal);
						toPose(1, 165, curr_z, gripperVal); // eraser final pose
						toPose(1, 165, 170+30, gripperVal);
					}
					if (gripperVal == 130)
					{
						toPose(20, 165, curr_z, gripperVal); // orange final pose
						toPose(20, 165, 170, gripperVal);
					}
					delay_ms(1000);
					gripperCmd(180);
					return 2;//finished grasping
		//				toPose(170, 0, 240, 180);
				}
			}
		}
		return 0;
	}
}		


 
 void loading(void){
	LED1 = 1;
 }
 
 
 void unloading(void){
	LED1 = 0;
 }

