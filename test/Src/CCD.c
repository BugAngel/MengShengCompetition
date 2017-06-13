/**
  ******************************************************************************
  * @file    CCD.c
  * @author  WangRong
  * @version V1.0
  * @date    2016/3/31
  * @brief   CCD底层驱动
  **@attention
  * 实验平台:红树伟业第五代线型CCD
  ******************************************************************************
  */

#include "ccd.h"

extern ADC_HandleTypeDef hadc1;


/**
  * @brief  延时.
  * @param  NONE
  * @retval None
  */
void delay()
{
    unsigned int i;
    for (i = 0; i < 20; i++)
    {
        __ASM("nop");
    }
}

/**
  * @brief  CCD初始化
  * @param  None
  * @retval None
  */
void CCD_Init(void)
{
    uint8_t i;
    HAL_GPIO_WritePin(CCD_SI_GPIO_Port, CCD_SI_Pin | CCD_CLK_Pin, GPIO_PIN_SET);

    /*初始化 不进行采样*/
    SI1_SetVal();
    CLK1_SetVal();
    SI1_ClrVal();
    for (i = 0; i < 128; i++)
    {
        CLK1_ClrVal();
        CLK1_SetVal();
    }
    CLK1_ClrVal();
}

/**
  * @brief  CCD1采集128个点.
  * @param  CCD_Info结构体
  * @retval None
  */
void CCD_ImageCapture(CCD_Info *CCD1_info)
{
    uint8_t i;
    SI1_SetVal();

    CLK1_SetVal();

    SI1_ClrVal();
    HAL_ADC_Start(&hadc1);
    CCD1_info->PixelOri[0][0] =  HAL_ADC_GetValue(&hadc1);

    CLK1_ClrVal();
    for (i = 1; i < 128; i++)
    {
        CLK1_SetVal();
        HAL_ADC_Start(&hadc1);
        CCD1_info->PixelOri[0][i] =  HAL_ADC_GetValue(&hadc1);
        CLK1_ClrVal();

    }
    CLK1_SetVal();

    CLK1_ClrVal();



}
