/**
  ******************************************************************************
  * @file    Blue_Tooth.c
  * @author  WangRong
  * @version V1.1
  * @date    2016.10.1
  * @note    蓝牙WIFI都可以用
  ******************************************************************************
  */
#include "bluetooth.h"

extern UART_HandleTypeDef huart1;

uint8_t USART_RX_BUF[USART_REC_LEN] = {0};   //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，   接收完成标志
//bit14，   接收到0x0d
//bit13~0， 接收到的有效字节数目
uint16_t USART_RX_STA = 0;     //接收状态标记

extern  uint8_t HandshakeSignalFirstSignal;
extern  uint8_t HandshakeSignalThirdSignal;
extern  uint8_t Judge[4];
/**
* @brief  接收扫描单片机的数据
* @param  usart_receive 接收串口数据
  * @arg  第一位  0x00  检验
    * @arg  第二位  KEY1~8
    * @arg  第三位  KEY9~16
    * @arg  第四位  KEY17~18
    * @arg  第五位  0x0d  检验
    * @arg  第六位  0x0a  检验
* @retval NONE
* @attention
*/
void ScanReceive(uint8_t *usart_receive)
{

    if ((USART_RX_STA & 0x8000) == 0) //接收未完成
    {
        if (USART_RX_STA & 0x4000) //接收到了0x0d
        {
            if (*usart_receive != 0x0a)
            {
                USART_RX_STA = 0; //接收错误,重新开始
            }
            else
            {
                USART_RX_STA |= 0x8000; //接收完成了
            }
        }
        else //还没收到0X0D
        {
            if (*usart_receive == 0x0d)
            {
                USART_RX_STA |= 0x4000;
            }
            else
            {
                USART_RX_BUF[USART_RX_STA & 0X3FFF] = *usart_receive ;
                USART_RX_STA = USART_RX_STA + 1;
                if (USART_RX_STA > (USART_REC_LEN - 1))
                {
                    USART_RX_STA =  0; //接收数据错误,重新开始接收
                }
            }
        }
    }

    if (USART_RX_STA & 0x8000)
    {
        //检验第一位  确保数据正确
        if (USART_RX_BUF[0] == 0xEE)
        {
            if (USART_RX_BUF[1] == 0xf0)
            {
                HandshakeSignalFirstSignal = 1;
            }

            else if (USART_RX_BUF[1] == 0x0f)
            {
                HandshakeSignalThirdSignal = 1;
            }
            else if ((USART_RX_BUF[1] | 0x06) == 1)
            {
                Judge[0] = 1;
            }

            else if ((USART_RX_BUF[1] | 0x18) == 1)
            {
                Judge[1] = 1;
            }
            else if ((USART_RX_BUF[1] | 0x60) == 1)
            {
                Judge[2] = 1;
            }
            else if (((USART_RX_BUF[1] | 0x80) == 1) || ((USART_RX_BUF[2] | 0x04) == 1))
            {
                Judge[3] = 1;
            }
        }
        USART_RX_STA = 0;
    }


}


