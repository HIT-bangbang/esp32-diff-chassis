#include "kinematics.h"
#include "commen.h"

Kinematics::Kinematics(float _width, float _radius)
{
    width = _width;
    radius = _radius;
}

/**
 * @brief: 运动学正解： 当前实际轮子转速 ----- 地盘实际的线速度角速度
 * @param {float} omega_A   车轮A的转速，单位rad/s
 * @param {float} omega_B
 * @return {*}
 */
void Kinematics::forward(float omega_A, float omega_B)
{
    real_linear_vel = 0.5 * radius * (omega_A + omega_B);
    real_angular_vel = (-omega_A + omega_B) / width;
}


/**
 * @brief: 上位机发来的指令线速度和角速度 ----> 两个轮子的期望转速 rad/s
 * @param {float} cmd_linear 线速度 m/s
 * @param {float} cmd_angular 角速度 m/s
 * @return {*}
 */
void Kinematics::inverse(float cmd_linear, float cmd_angular)
{
    target_omega_A =  (cmd_linear - cmd_angular * width / 2.0) / radius;
    target_omega_B =  (cmd_linear + cmd_angular * width / 2.0) / radius;
}

float Kinematics::get_real_linear_vel()
{
    return real_linear_vel;
}


float Kinematics::get_real_angular_vel()
{
    return real_angular_vel;
}