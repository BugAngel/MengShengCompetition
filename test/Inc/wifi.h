/**
  ******************************************************************************
  * @file    wifi.c
  * @author  WangRong
  * @version V1.1
  * @date    2016.10.1
  * @note    蓝牙WIFI都可以用
  ******************************************************************************
  */
#ifndef _WIFI_H_
#define _WIFI_H_

#include "stm32f4xx_hal.h"

#define WIFI_REC_LEN  			20  	//定义最大接收字节数 20

/***************发送数据类型***************/
#define CPUStatus 0x32
#define LeftRoom 0x33
#define MiddleLine 0x34
#define LSpeed 0x35
#define RSpeed 0x36
#define Pitch 0x37
#define Yaw 0x38
#define Roll 0x39
#define Number1 0x3A
#define Number2 0x3B
#define Number3 0x3C
#define Number4 0x3D
#define Number5 0x3E
#define Number6 0x3F
#define CCD1 0x40
#define CCD2 0x41
#define CMOS 0x42
#define JUDGE 0x43
#define OK 0x44
/***************接收数据类型***************/
#define  Rec_Balance_P 0x20
#define  Rec_Balance_I 0x21
#define  Rec_Balance_D 0x22
#define  Rec_Position_P 0x23
#define  Rec_Position_I 0x24
#define  Rec_Position_D 0x25
#define  Rec_Speed_P 0x26
#define  Rec_Speed_I 0x27
#define  Rec_Speed_D 0x28
#define  Rec_Steer_P 0x29
#define  Rec_Steer_I 0x2A
#define  Rec_Steer_D 0x2B
#define  Rec_Forward  0x2C
#define  Rec_Backward  0x2D
#define  Rec_TurnLeft  0x2E
#define  Rec_TurnRight  0x2F
#define  Rec_Firing  0x30
#define  Rec_Misfire  0x31
#define  Rec_EncoderForward 0x32
#define  Rec_EncoderBackward  0x33
#define  Rec_EncoderTurnLeft  0x34
#define  Rec_EncoderTurnRight 0x35
#define  Rec_EncoderFiring  0x36
#define  Rec_EncoderMisfire 0x37
#define  Rec_YAW 0x38
#define  Rec_SetSpeed 0x39

void UpperComputer(uint8_t *usart_receive);
void Send_Uint8_Data(uint8_t data, uint8_t type);
void Send_Float_Data(float data, uint8_t type);
void Send_CCD_Data(uint8_t data[], uint8_t type);

#endif  /* _Control_H_ */
