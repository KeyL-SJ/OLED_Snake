#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
int Read_Encoder(u8 TIMx);

int My_Abs(int a);
int Encoder_Choose(int encoder_left,int encoder_right);
#endif