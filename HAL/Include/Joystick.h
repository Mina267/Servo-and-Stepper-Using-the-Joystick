/*
 * Joystick.h
 *
 * Created: 4/15/2023 12:35:55 AM
 *  Author: mina2
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_


#define VX_CHANNEL	CH_0
#define VY_CHANNEL	CH_7

void Joystick_Init(void);

void Joystick_Runnable(void);
void Calc_Angle(void);

void Joystick_Display(void);
u16 Joystick_GetAngle(void);
u32 abs_num(s32 num);
void Search_ceta(u32 num);
u8 SmallestOfThree(u16 n1, u16 n2, u16 n3);
void Angle_FilterInit(void);
void Angle_FilterRunnable(void);
#endif /* JOYSTICK_H_ */