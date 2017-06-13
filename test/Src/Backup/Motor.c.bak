/**
******************************************************************************
    * @file     Motor.c
    * @author   WangRong
    * @version  1.0
    * @date     2016/10/31
    * @brief    Motor Driver
            @arg    TIM1 CH1 2控制左后电机
                        @arg    TIM1 CH3 4控制左前电机
                        @arg    TIM8 CH1 3控制右前电机
                        @arg    TIM8 CH2 4控制右后电机
******************************************************************************
*/

#include "Motor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;
extern uint8_t MotorFlag;       //小车启动信号
/**
  * @brief  单片机PWM功能开启
  * @param  NONE
  * @retval NONE
  */
void Motor_init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

/**
  * @brief  左后电机控制
  * @param  speed_value:预期PWM CCR寄存器值
  * @retval NONE
  */
void MotorSpeedLB(int32_t speed_value)
{

    if (speed_value > 0)     //正转
    {
        if (speed_value > MOTOR_SPEED_MAX)
        {
            speed_value = MOTOR_SPEED_MAX;
        }
        if (speed_value < MOTOR_SPEED_MIN)
        {
            speed_value = MOTOR_SPEED_MIN;
        }
        TIM1->CCR1 = speed_value;
        TIM1->CCR2 = 0;
    }
    else                       //反转
    {
        if (speed_value < -MOTOR_SPEED_MAX)
        {
            speed_value = -MOTOR_SPEED_MAX;
        }
        if (speed_value > -MOTOR_SPEED_MIN)
        {
            speed_value = -MOTOR_SPEED_MIN;
        }
        TIM1->CCR1 = 0;
        TIM1->CCR2 = -speed_value;
    }
}

/**
  * @brief  左前电机控制
  * @param  speed_value:预期PWM CCR寄存器值
  * @retval NONE
  */
void MotorSpeedLF(int32_t speed_value)
{

    if (speed_value > 0)     //正转
    {
        if (speed_value > MOTOR_SPEED_MAX)
        {
            speed_value = MOTOR_SPEED_MAX;
        }
        if (speed_value < MOTOR_SPEED_MIN)
        {
            speed_value = MOTOR_SPEED_MIN;
        }


        TIM1->CCR4 = speed_value;
        TIM1->CCR3 = 0;
    }
    else                       //反转
    {
        if (speed_value < -MOTOR_SPEED_MAX)
        {
            speed_value = -MOTOR_SPEED_MAX;
        }
        if (speed_value > -MOTOR_SPEED_MIN)
        {
            speed_value = -MOTOR_SPEED_MIN;
        }
        TIM1->CCR4 = 0;
        TIM1->CCR3 = -speed_value;
    }
}

/**
  * @brief  右前电机控制
  * @param  speed_value:预期PWM CCR寄存器值
  * @retval NONE
  */
void MotorSpeedRF(int32_t speed_value)
{

    if (speed_value > 0)     //正转
    {
        if (speed_value > MOTOR_SPEED_MAX)
        {
            speed_value = MOTOR_SPEED_MAX;
        }
        if (speed_value < MOTOR_SPEED_MIN)
        {
            speed_value = MOTOR_SPEED_MIN;
        }
        TIM8->CCR1 = speed_value;
        TIM8->CCR3 = 0;
    }
    else                       //反转
    {
        if (speed_value < -MOTOR_SPEED_MAX)
        {
            speed_value = -MOTOR_SPEED_MAX;
        }
        if (speed_value > -MOTOR_SPEED_MIN)
        {
            speed_value = -MOTOR_SPEED_MIN;
        }
        TIM8->CCR1 = 0;
        TIM8->CCR3 = -speed_value;
    }
}
/**
  * @brief  右后电机控制
  * @param  speed_value:预期PWM CCR寄存器值
  * @retval NONE
  */
void MotorSpeedRB(int32_t speed_value)
{

    if (speed_value > 0)     //正转
    {
        if (speed_value > MOTOR_SPEED_MAX)
        {
            speed_value = MOTOR_SPEED_MAX;
        }
        if (speed_value < MOTOR_SPEED_MIN)
        {
            speed_value = MOTOR_SPEED_MIN;
        }
        TIM8->CCR2 = speed_value;
        TIM8->CCR4 = 0;
    }
    else                       //反转
    {
        if (speed_value < -MOTOR_SPEED_MAX)
        {
            speed_value = -MOTOR_SPEED_MAX;
        }
        if (speed_value > -MOTOR_SPEED_MIN)
        {
            speed_value = -MOTOR_SPEED_MIN;
        }
        TIM8->CCR2 = 0;
        TIM8->CCR4 = -speed_value;
    }
}
/**
  * @brief  停车
  * @param  NONE
  * @retval NONE
  */
void Stop(void)
{
    MotorFlag = 0;
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;
    TIM1->CCR4 = 0;
    TIM8->CCR1 = 0;
    TIM8->CCR3 = 0;
    TIM8->CCR2 = 0;
    TIM8->CCR4 = 0;
}



