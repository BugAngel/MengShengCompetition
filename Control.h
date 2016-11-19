/**
  ******************************************************************************
  * @file    control.h
  * @author  WangRong
  * @version V3.3
  * @date    2016/1/13
  * @brief   
  ******************************************************************************
  */


#ifndef _Control_H_
#define _Control_H_

#include "stm32f4xx_hal.h"
#include "pid.h"

void PID_init(void);
//void PIDStop(void);
void UpperSpeedControl(int32_t UpperSetSpeed);
void Pid_Control(float yaw, float UpperSetYaw,int32_t UpperSetSpeed);
void GyrateRight(void);
void GyrateLeft(void);
#endif  /* _Control_H_ */

