#ifndef __PI_TRUN__
#define __PI_TRUN__



typedef struct
{
	float inte;
	float ekk;
	float Kp;
	float Ki;
	float set;
	float Ur;
}pi;

void pi_init(pi *);

void pi_turn();

float pi_cal(float yaw, pi *p);

#endif