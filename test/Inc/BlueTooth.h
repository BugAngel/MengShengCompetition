/**
  ******************************************************************************
  * @file    Blue_Tooth.h
  * @author  WangRong
  * @version V1.0
  * @date    2016.8.17
  * @note    串口蓝牙
  ******************************************************************************
  */
#ifndef __BLUE_TOOTH_H
#define __BLUE_TOOTH_H

#include "stm32f4xx_hal.h"

#define USART_REC_LEN  			20  	//定义最大接收字节数 20
void ScanReceive(uint8_t *usart_receive);
#endif
