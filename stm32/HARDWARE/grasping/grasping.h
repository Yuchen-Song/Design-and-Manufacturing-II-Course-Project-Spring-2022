#ifndef _GRASPING_H
#define _GRASPING_H
#include "sys.h"

void fkine(u16 p1, u16 p2, u16 p3, u16 p4);
void ikine(signed short int x, signed short int y, signed short int z, signed short int gripper);
void initPose(signed short int tar_g);
void toPose(signed short int tar_x, signed short int tar_y, signed short int tar_z, signed short int tar_g);
void fromZ(signed short int _x, signed short int _y, signed short int _z);
void gripperCmd(signed short int val);
int graspingStage(int Flag);
void loading(void);
void unloading(void);

#endif