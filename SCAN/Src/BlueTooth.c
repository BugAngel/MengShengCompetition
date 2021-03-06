/**
  ******************************************************************************
  * @file    Blue_Tooth.h
  * @author  WangRong
  * @version V1.0
  * @date    2016.10.31
  * @note    串口蓝牙
  ******************************************************************************
  */
#include "BlueTooth.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/**
  * @brief  发uint8_t类型数据据至上位机
  * @param  NONE
  * @retval NONE
  * @attention
        通讯协议
    * @arg  第一位  0x00  检验
        * @arg  第二位  key1~8
        * @arg  第三位  key9~16
        * @arg  第四位  key17~18
        * @arg  第五位  0x0d  检验
        * @arg  第六位  0x0a  检验
  */
void SendData(void)
{
    uint8_t Scan1, Scan2, Scan3;
    uint8_t (*Scan)[3];

    /*函数返回指向数组的指针*/
    Scan = KEY_Scan();
    //赋值
    Scan1 = (*Scan)[0];
    Scan2 = (*Scan)[1];
    Scan3 = (*Scan)[2];
    /*释放空间*/
    free(Scan);
    /*串口发送*/
    uint8_t SendData[6] = {0x00, Scan1, Scan2, Scan3, 0x0d, 0x0a};
    HAL_UART_Transmit(&huart1 , SendData, 6, 0xff);
}


