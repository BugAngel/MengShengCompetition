#ifndef _PID_H_
#define _PID_H_


typedef struct
{
    //PID 三个参数
    float Kp;
    float Ki;
    float Kd;
    //积分分离
    float beta;//0就是不带积分分离， 如果不为零那么就是积分范围
    //微分先行
    int dif_prior;//0是不带微分先行
    //变速积分
    //PID输出值
    float last_U; //上一次的输出值
    float delta_U;//增量式PID
    float U;//位置式PID
    //设定值
    float s[3];
    //误差
    float e[3];//每次的误差2是最新的1是上一次的0是大上次
    //实际值
    float r[3];
    //前馈参数
    float Kvff;
    float Kaff;

} PidTypeDef;

typedef struct
{
    float Kp_C;
    float Ki_C;
    float Kd_C;
    float s_C[3];
    float e_C[3];//每次的误差2是最新的1是上一次的0是大上次
    //实际值
    float r_C[3];
    float last_U_C; //上一次的输出值
    float delta_U_C;//增量式PID
    float U_C;//位置式PID
} PidCurrentTypeDef;

typedef struct
{
    float w11;
    float w12;
    float w21;
    float w22;
    float w31;
    float w32;
    float q1[2];
    float q2[2];
    float q3[2]; //隐层的三个神经元，分别代表比例，积分，微分部分
    float x1[2]; //
    float x2[2];//
    float x3[2];//隐层输入
    float out;
} nn_PidTypeDef;

void PID_Init(PidTypeDef * pid);

void PID_SetParam(PidTypeDef * pid, float p, float i, float d, float beta,
                  float dif_prior, float kaff, float kvff);
void PID_CurrentParam(PidCurrentTypeDef * pid, float p, float i, float d);

void PID_SpeedCalc(PidTypeDef * pid, float rel_val, float set_val);
void PID_PositionCalc(PidTypeDef * pid, float rel_val, float set_val);
void PID_CurrentCalc(PidCurrentTypeDef * pid, float rel_val, float set_val);



#endif
