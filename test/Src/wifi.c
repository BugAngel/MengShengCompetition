/**
  ******************************************************************************
  * @file    wifi.c
  * @author  WangRong
  * @version V1.1
  * @date    2016.10.1
  * @note    蓝牙WIFI都可以用
  ******************************************************************************
  */
#include "wifi.h"
#include "Motor.h"
#include "pid.h"
#include "control.h"

extern UART_HandleTypeDef huart3;

uint8_t WIFI_RX_BUF[WIFI_REC_LEN] = {0};   //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，   接收完成标志
//bit14，   接收到0x0d
//bit13~0， 接收到的有效字节数目
uint16_t WIFI_RX_STA = 0;     //接收状态标记

extern uint8_t MotorFlag;       //小车启动信号
extern int32_t UpperSetSpeed;
extern PidTypeDef RFMotorPID;
extern PidTypeDef LBMotorPID;
extern PidTypeDef LFMotorPID;
extern PidTypeDef RBMotorPID;
extern PidTypeDef YawPID;
extern float UpperSetYaw;//设定角度

/**
  * @brief  接收上位机数据
  * @param  usart_receive 接收串口数据
      * @arg  第一位  0xff  检验
        * @arg  第二位  对应数据
        * @arg  第三位  数据整数部分
        * @arg  第四位  数据第一二位小数
        * @arg  第五位  0x0d  检验
        * @arg  第六位  0x0a  检验
    * @retval NONE
  * @attention
    @arg
  */
void UpperComputer(uint8_t *usart_receive)
{
    float usart_temp;
    uint8_t len;
    if ((WIFI_RX_STA & 0x8000) == 0) //接收未完成
    {
        if (WIFI_RX_STA & 0x4000) //接收到了0x0d
        {
            if (*usart_receive != 0x0a)
            {
                WIFI_RX_STA = 0; //接收错误,重新开始
            }
            else
            {
                WIFI_RX_STA |= 0x8000;  //接收完成了
            }
        }
        else //还没收到0X0D
        {
            if (*usart_receive == 0x0d)
            {
                WIFI_RX_STA |= 0x4000;
            }
            else
            {
                WIFI_RX_BUF[WIFI_RX_STA & 0X3FFF] = *usart_receive ;
                WIFI_RX_STA++;
                if (WIFI_RX_STA > (WIFI_REC_LEN - 1))
                {
                    WIFI_RX_STA =  0; //接收数据错误,重新开始接收
                }
            }
        }
    }

    if (WIFI_RX_STA & 0x8000)
    {
        len = WIFI_RX_STA & 0x3fff; //得到此次接收到的数据长度
        //检验第一位  确保数据正确
        if (WIFI_RX_BUF[0] == 0xff)
        {
            if (len == 5)
            {
                usart_temp = (float)(WIFI_RX_BUF[3]) + ((float)WIFI_RX_BUF[4]) / 100;
            }
            if (WIFI_RX_BUF[2] == 0x01)
            {
                usart_temp = -usart_temp;
            }
            switch (WIFI_RX_BUF[1])
            {
                case Rec_Balance_P:

                    break;
                case Rec_Balance_I:

                    break;
                case Rec_Balance_D:

                    break;
                case Rec_Position_P:
                    YawPID.Kp = usart_temp;
                    break;
                case Rec_Position_I:
                    YawPID.Ki = usart_temp;
                    break;
                case Rec_Position_D:
                    YawPID.Kd = usart_temp;
                    break;
                case Rec_Speed_P:
                    RFMotorPID.Kp = usart_temp;
                    LBMotorPID.Kp = usart_temp;
                    LFMotorPID.Kp = usart_temp;
                    RBMotorPID.Kp = usart_temp;
                    break;
                case Rec_Speed_I:
                    RFMotorPID.Ki = usart_temp;
                    LBMotorPID.Ki = usart_temp;
                    LFMotorPID.Ki = usart_temp;
                    RBMotorPID.Ki = usart_temp;
                    break;
                case Rec_Speed_D:
                    RFMotorPID.Kd = usart_temp;
                    LBMotorPID.Kd = usart_temp;
                    LFMotorPID.Kd = usart_temp;
                    RBMotorPID.Kd = usart_temp;
                    break;
                case Rec_Steer_P:

                    break;
                case Rec_Steer_I:

                    break;
                case Rec_Steer_D:

                    break;
                case Rec_Forward:
                    MotorSpeedLB(500);
                    MotorSpeedLF(500);
                    MotorSpeedRF(500);
                    MotorSpeedRB(500);
                    break;
                case Rec_Backward:
                    MotorSpeedLB(-500);
                    MotorSpeedLF(-500);
                    MotorSpeedRF(-500);
                    MotorSpeedRB(-500);
                    break;
                case Rec_TurnLeft:

                    break;
                case Rec_TurnRight:

                    break;
                case Rec_Firing:
                    MotorFlag = 1;
                    break;
                case Rec_Misfire:
                    Stop();
                    break;
                case Rec_EncoderForward:
                    UpperSetSpeed = 20;
                    break;
                case Rec_EncoderBackward:
                    UpperSetSpeed = -20;
                    break;
                case Rec_EncoderTurnLeft:

                    break;

                case Rec_EncoderTurnRight:

                    break;
                case Rec_EncoderFiring:
                    MotorFlag = 3;
                    break;
                case Rec_EncoderMisfire:
                    Stop();
                    break;
                case Rec_YAW:
                    UpperSetYaw = usart_temp;
                    break;
                case Rec_SetSpeed:
                    UpperSetSpeed = usart_temp;
                    break;
            }
        }

        WIFI_RX_STA = 0;
    }



}
/**
  * @brief  发uint8_t类型数据据至上位机
  * @param
    * @arg  data  要发送的数据
    * @arg  type  发送数据类型
    * @retval NONE
  * @attention
        通讯协议
    * @arg  第一位  0xff  检验
        * @arg  第二位  对应数据类型
        * @arg  第三位  要发送的数据
        * @arg  第四位  0x0d  检验
        * @arg  第五位  0x0a  检验
  */
void Send_Uint8_Data(uint8_t data, uint8_t type)
{
    uint8_t SendData[5] = {0xff, type, data, 0x0d, 0x0a};
    HAL_UART_Transmit(&huart3 , SendData, 5, 0xff);
}
/**
  * @brief  发float类型数据据至上位机
  * @param
    * @arg  data  要发送的数据
    * @arg  type  发送数据类型
    * @retval NONE
  * @attention
        通讯协议
    * @arg  第一位  0xff  检验
        * @arg  第二位  对应数据类型
        * @arg  第三位  判断正负
        * @arg  第四位  要发送的数据整数位
        * @arg  第五位  要发送的数据小数位
        * @arg  第六位  0x0d  检验
        * @arg  第七位  0x0a  检验
  */
void Send_Float_Data(float data, uint8_t type)
{
    uint8_t SendData[7] = {0xff, type, 0x00, 0x00, 0x00, 0x0d, 0x0a};
    if (data < 0)
    {
        SendData[2] = 0x01;
        data = -data;
    }
    //temp_1整数部分  temp_2两位小数
    uint8_t temp_1, temp_2;
    temp_1 = (uint8_t)data;
    SendData[3] = temp_1;
    temp_2 = (uint8_t)(100 * (data - (float)temp_1));
    SendData[4] = temp_2;

    HAL_UART_Transmit(&huart3 , SendData, 7, 0xff);
}

/**
  * @brief  发CCD数据据至上位机
  * @param
    * @arg  data  要发送的数据  128位数组
    * @arg  type  发送数据类型
    * @retval NONE
  * @attention
        通讯协议
    * @arg  第一位  0x0f  检验
    * @arg  第二位  type  数据类型
        * @arg  倒数第二位  0x0d  检验
        * @arg  倒数第一位  0x0a  检验
        * @arg  其余128位数据位
  */
void Send_CCD_Data(uint8_t data[], uint8_t type)
{
    uint8_t temp_1 = 0x0f;
    uint8_t temp_2[2] = {0x0d, 0x0a};
    HAL_UART_Transmit(&huart3 , &temp_1, 1, 0xff);
    HAL_UART_Transmit(&huart3 , &type, 1, 0xff);
    HAL_UART_Transmit(&huart3 , data, 128, 0xffff);
    HAL_UART_Transmit(&huart3 , temp_2, 2, 0xff);
}

