/**
  ******************************************************************************
  * @file    key.c
  * @author  WangRong
  * @version V1.0
  * @date    2016.10.31
  * @note    红外扫描
  ******************************************************************************
  */
#include <stdio.h>
#include <stdlib.h>
#include "key.h"

/**
  * @brief  检测红外对管状态
  * @param  NONE
  * @retval 数组指针
  * @attention
       @arg 第一个数7~0位对应KEY8~1
             @arg 第二个数7~0位对应KEY16~9
             @arg 第三个数1~0位对应KEY18~17
  */
uint8_t (*KEY_Scan(void))[3]
{
    uint8_t (*Send)[3];
    /*动态开辟空间*/
    Send = (uint8_t (*)[3])calloc(3, sizeof(uint8_t));

//    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[0] |= 0x01;
//    }
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x02;
    }
    if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x04;
    }
    if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x08;
    }
    if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x10;
    }
    if (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x20;
    }
    if (HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x40;
    }
    if (HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[0] |= 0x80;
    }
//    if (HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x01;
//    }
//    if (HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x02;
//    }
    if (HAL_GPIO_ReadPin(KEY11_GPIO_Port, KEY11_Pin) == GPIO_PIN_RESET)
    {
        (*Send)[1] |= 0x04;
    }
//    if (HAL_GPIO_ReadPin(KEY12_GPIO_Port, KEY12_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x08;
//    }
//    if (HAL_GPIO_ReadPin(KEY13_GPIO_Port, KEY13_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x10;
//    }
//    if (HAL_GPIO_ReadPin(KEY14_GPIO_Port, KEY14_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x20;
//    }
//    if (HAL_GPIO_ReadPin(KEY15_GPIO_Port, KEY15_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x40;
//    }
//    if (HAL_GPIO_ReadPin(KEY16_GPIO_Port, KEY16_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[1] |= 0x80;
//    }
//    if (HAL_GPIO_ReadPin(KEY17_GPIO_Port, KEY17_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[2] |= 0x01;
//    }
//    if (HAL_GPIO_ReadPin(KEY18_GPIO_Port, KEY18_Pin) == GPIO_PIN_RESET)
//    {
//        (*Send)[2] |= 0x02;
//    }
    return Send;
}

