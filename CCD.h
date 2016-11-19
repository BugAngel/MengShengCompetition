/**
  ******************************************************************************
  * @file    CCD.h
  * @author  WangRong
  * @version V1.0
  * @date    2016/3/31
  * @brief   CCD底层驱动
  **@attention
  * 实验平台:红树伟业第五代线型CCD
  ******************************************************************************
  */

#ifndef __CCD_H
#define __CCD_H

#include "stm32f4xx_hal.h"

#define SI1_SetVal()   HAL_GPIO_WritePin(CCD_SI_GPIO_Port,CCD_SI_Pin, GPIO_PIN_SET)
#define SI1_ClrVal()   HAL_GPIO_WritePin(CCD_SI_GPIO_Port,CCD_SI_Pin, GPIO_PIN_RESET)
#define CLK1_SetVal()  HAL_GPIO_WritePin(CCD_CLK_GPIO_Port,CCD_CLK_Pin, GPIO_PIN_SET)
#define CLK1_ClrVal()  HAL_GPIO_WritePin(CCD_CLK_GPIO_Port,CCD_CLK_Pin, GPIO_PIN_RESET)

typedef struct CCD_Info_//--------------------------------CCD数据
{
    uint16_t PixelOri[2][128];          // CCD原始值
    uint16_t Pixel[128];                // CCD滤波后值
    uint8_t PixelBinary[128];          // CCD二值化值

		uint16_t AD_MAX[4];					//AD最大值
	uint16_t AD_MIN[4];					//AD最小值
	
    uint8_t Pixel_2[128];              //CCD图像第二次滤波值
    int16_t CCD_LWideth[10];
    int16_t CCD_RWideth[10];           //边界与上次中点误差
    int16_t CCD_Wideth[10];	
    uint16_t CCD_Left[10];           //CCD扫到白线左边坐标
    uint16_t CCD_Right[10];          //CCD扫到白线右边坐标
    uint16_t Right_temp;                //CCD右边线临时值
    uint16_t Left_temp;                 //CCD左边线临时值

    int16_t  LeftLine[5];       //左边界队列
    int16_t  CentralLine[5];    //中线队列
    int16_t  RightLine[5];      //右边界队列
    int16_t  LineError[5];      //偏差队列
    int16_t  RoadWidth[5];      //路宽队列

    uint8_t LeftLoseLine_Flag;
    uint8_t RightLoseLine_Flag;
    uint8_t LossLine_Flag;
    int16_t  RoadWidthOfStraight;       //直道的路宽
  
    uint8_t Cross_Flag;                 //十字道标志
    uint8_t  BurstNumber;
	float CCD_AD_TargetValue;			//CCD电压放大的目标值
	uint16_t CCD_Binary_MIN;
	uint16_t CCD_Binary_MAX;
} CCD_Info;

void CCD_Init(void);
void CCD_ImageCapture(CCD_Info *CCD1_info);
#endif
