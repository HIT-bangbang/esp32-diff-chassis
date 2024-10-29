#ifndef COMMEN_H
#define COMMEN_H


// ! 电机

// 定义频率、通道、分辨率和LED引脚

#define FREQ 1000

#define MA_PWM_CHANNEL 1
#define MB_PWM_CHANNEL 2

#define RESOLUTION 8

#define MA_ENB 4
#define MA_IN1 16
#define MA_IN2 17
#define MA_DIR 0        // 电机A方向，取值为 0 或 1

#define MB_ENB 21
#define MB_IN1 18
#define MB_IN2 19
#define MB_DIR 1        // 电机B方向，取值为 0 或 1


//! 编码器

#define MA_ENC_A 13
#define MA_ENC_B 14
#define MA_ENC_DIR 1    // 编码器A方向，取值为 -1 或 1

#define MB_ENC_A 26
#define MB_ENC_B 27
#define MB_ENC_DIR 1    // 编码器B方向，取值为 -1 或 1

#define ENC_RES 30*44     // 轮子转一圈有多少脉冲 = 减速比*编码器分辨率

// ! pid

#define KP 0.1       // PID_P
#define KI 0.2       // PID_I
#define KD 0.0       // PID_D
#define RAMP -1.0       // PID 斜率限制 0.0及负值表示无斜率限制
#define PID_LIMIT 1.0       // PID 积分限幅 和 输出限幅



//! 底盘机械属性

#define WIDTH 0.2       // 轮距
#define RADIUS 0.05     // 轮子直径



#define _2PI 6.2831853

//限幅
#define _constrain(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))

#endif


