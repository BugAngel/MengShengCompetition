/**
  ******************************************************************************
  * @file    control.h
  * @author  WangRong
  * @version V1.0
  * @date    2016/1/13
  * @brief   control
  * @note    YAW 右转减小，左转增加
  ******************************************************************************
  */
#include "stm32f4xx_hal.h"
#include "pid.h"
#include "Control.h"
#include "Motor.h"

extern PidTypeDef RFMotorPID;
extern PidTypeDef LBMotorPID;
extern PidTypeDef LFMotorPID;
extern PidTypeDef RBMotorPID;
extern PidTypeDef YawPID;    //角度PID

extern int16_t MotorSpeed1;//右前  正转负数，倒转正数
extern int16_t MotorSpeed2;//左后  正转负数，倒转正数
extern int16_t MotorSpeed3;//左前  正转正数，倒转负数
extern int16_t MotorSpeed4;//右后  正转负数，倒转正数

extern int32_t UpperSetSpeed;
extern uint8_t MotorFlag;       //小车启动信号
extern int32_t LeftForwardSpeed;//左前轮改变速度
extern int32_t RightForwardSpeed;//右前轮改变速度

uint32_t RBtemp = 0; //右后临时PWM
uint32_t RFtemp = 0; //右前临时PWM
uint32_t LBtemp = 0; //左后临时PWM
uint32_t LFtemp = 0; //左前临时PWM

void PID_init(void)
{
    PID_SetParam(&RFMotorPID, 2, 0, -0.4, 0, 0, 0, 0);  //右前电机闭环PID
    PID_SetParam(&LBMotorPID, 2, 0, -0.4, 0, 0, 0, 0);  //左后电机闭环PID
    PID_SetParam(&LFMotorPID, 2, 0, -0.4, 0, 0, 0, 0);  //左前电机闭环PID
    PID_SetParam(&RBMotorPID, 2, 0, -0.4, 0, 0, 0, 0);  //右后电机闭环PID
    PID_SetParam(&YawPID, 2, 0, -0.4, 0, 0, 0, 0);  //角度闭环PID

}
/**
  * @brief  PID控制停车
  * @param  NONE
  * @retval NONE
  */
void PIDStop(void)
{
    MotorFlag = 0;
    PID_SpeedCalc(&RFMotorPID, MotorSpeed1, 0);
    MotorSpeedRF((int32_t)RFMotorPID.U);  //右前电机转速控制

    PID_SpeedCalc(&LBMotorPID, MotorSpeed2, 0);
    MotorSpeedLB((int32_t)LBMotorPID.U);  //左后电机转速控制

    PID_SpeedCalc(&LFMotorPID, -MotorSpeed3, 0);
    MotorSpeedLF((int32_t)LFMotorPID.U);  //左前电机转速控制

    PID_SpeedCalc(&RBMotorPID, MotorSpeed4, 0);
    MotorSpeedRB((int32_t)RBMotorPID.U);  //右后电机转速控制

}
/**
  * @brief  控制前后转时电机转速
  * @param  UpperSetSpeed:设定编码器速度
  * @retval NONE
  */
void UpperSpeedControl(int32_t UpperSetSpeed)
{
    PID_SpeedCalc(&RFMotorPID, -MotorSpeed1, UpperSetSpeed);
    RFtemp += (int32_t)RFMotorPID.U;
    MotorSpeedRF(RFtemp);  //右前电机转速控制

    PID_SpeedCalc(&LBMotorPID, -MotorSpeed2, UpperSetSpeed);
    LBtemp += (int32_t)LBMotorPID.U;
    MotorSpeedLB(LBtemp);  //左后电机转速控制

    PID_SpeedCalc(&LFMotorPID, MotorSpeed3, UpperSetSpeed);
    LFtemp += (int32_t)LFMotorPID.U;
    MotorSpeedLF(LFtemp);  //左前电机转速控制

    PID_SpeedCalc(&RBMotorPID, -MotorSpeed4, UpperSetSpeed);
    RBtemp += (int32_t)RBMotorPID.U;
    MotorSpeedRB(RBtemp);  //右后电机转速控制
}

/**
  * @brief  PID控制
  * @param
                    @arg  Yaw:当前角度
                    @arg SetYaw :设定角度
  * @retval NONE
  */
void Pid_Control(float yaw, float UpperSetYaw, int32_t UpperSetSpeed)
{
    PID_PositionCalc(&YawPID, yaw, UpperSetYaw);
    LeftForwardSpeed = (int32_t)(UpperSetSpeed - YawPID.U / 10  );
    RightForwardSpeed = (int32_t)(UpperSetSpeed + YawPID.U / 10 );

    PID_SpeedCalc(&LFMotorPID, MotorSpeed3, LeftForwardSpeed);
    LFtemp += (int32_t)LFMotorPID.U;
    MotorSpeedLF(LFtemp);  //左前电机转速控制

    PID_SpeedCalc(&RFMotorPID, -MotorSpeed1, RightForwardSpeed);
    RFtemp += (int32_t)RFMotorPID.U;
    MotorSpeedRF(RFtemp);  //右前电机转速控制

    PID_SpeedCalc(&LBMotorPID, -MotorSpeed2, LeftForwardSpeed);
    LBtemp += (int32_t)LBMotorPID.U;
    MotorSpeedLB(LBtemp);  //左后电机转速控制

    PID_SpeedCalc(&RBMotorPID, -MotorSpeed4, RightForwardSpeed);
    RBtemp += (int32_t)RBMotorPID.U;
    MotorSpeedRB(RBtemp);  //右后电机转速控制


}

void GyrateRight(void)
{
    PID_SpeedCalc(&LFMotorPID, MotorSpeed3, 30);
    LFtemp += (int32_t)LFMotorPID.U;
    MotorSpeedLF(LFtemp);  //左前电机转速控制

    PID_SpeedCalc(&RFMotorPID, -MotorSpeed1, -30);
    RFtemp += (int32_t)RFMotorPID.U;
    MotorSpeedRF(RFtemp);  //右前电机转速控制

    PID_SpeedCalc(&LBMotorPID, -MotorSpeed2, 30);
    LBtemp += (int32_t)LBMotorPID.U;
    MotorSpeedLB(LBtemp);  //左后电机转速控制

    PID_SpeedCalc(&RBMotorPID, -MotorSpeed4, -30);
    RBtemp += (int32_t)RBMotorPID.U;
    MotorSpeedRB(RBtemp);  //右后电机转速控制
}

void GyrateLeft(void)
{
    PID_SpeedCalc(&RFMotorPID, -MotorSpeed1, -30);
    RFtemp += (int32_t)RFMotorPID.U;
    MotorSpeedRF(RFtemp);  //右前电机转速控制

    PID_SpeedCalc(&LBMotorPID, -MotorSpeed2, 30);
    LBtemp += (int32_t)LBMotorPID.U;
    MotorSpeedLB(LBtemp);  //左后电机转速控制

    PID_SpeedCalc(&LFMotorPID, MotorSpeed3, -30);
    LFtemp += (int32_t)LFMotorPID.U;
    MotorSpeedLF(LFtemp);  //左前电机转速控制

    PID_SpeedCalc(&RBMotorPID, -MotorSpeed4, 30);
    RBtemp += (int32_t)RBMotorPID.U;
    MotorSpeedRB(RBtemp);  //右后电机转速控制
}
