#include"encoder.h"
#include "Arduino.h"
#include <functional>
#include <commen.h>
#include "FunctionalInterrupt.h"

Encoder::Encoder(uint8_t _enc_A_pin, uint8_t _enc_B_pin, int8_t _direction, int _enc_res)
{
    Int_Callback_A = std::bind(&Encoder::Callback_A, this);
    Int_Callback_B = std::bind(&Encoder::Callback_B, this);

    enc_A_pin = _enc_A_pin;
    enc_B_pin = _enc_B_pin;
    direction = _direction;

    enc_res = _enc_res;
}

void Encoder::init()
{
    pinMode(enc_A_pin,INPUT);
    pinMode(enc_B_pin,INPUT);

    attachInterrupt(enc_A_pin,Int_Callback_A,CHANGE);   // #include "FunctionalInterrupt.h" 之后可以直接这样用
    attachInterrupt(enc_B_pin,Int_Callback_B,CHANGE);   // #include "FunctionalInterrupt.h" 之后可以直接这样用

    last_time = micros();
    last_count = count;
}

// A相捕获到边沿
void Encoder::Callback_A()
{
    bool stateA = digitalRead(enc_A_pin);
    bool stateB = digitalRead(enc_B_pin);

    if (stateA == stateB)
    {
        count += direction;
    }
    else if (stateA != stateB)
    {
        count -= direction;
    }
}

// B相捕获到边沿
void Encoder::Callback_B()
{
    bool stateA = digitalRead(enc_A_pin);
    bool stateB = digitalRead(enc_B_pin);

    if (stateA != stateB)
    {
        count += direction;
    }
    else if (stateA == stateB)
    {
        count -= direction;
    }
}

/**
 * @brief: 更新轮子 角速度
 * @return {*}
 */
void Encoder::update()
{
    unsigned long cur_time = micros();
    float interval_s = float(cur_time - last_time) * 1e-6; // 时间间隔 s
    omega = _2PI * float(count - last_count) / enc_res / interval_s; // 角速度rad/s = 2pi * 转/s

    last_time = cur_time;
    last_count = count;
}


/**
 * @brief: 获取count计数值
 * @return {*}
 */
int64_t Encoder::get_count()
{
    return count;
}

/**
 * @brief: 获得角速度
 * @return {*}
 */
float Encoder::get_omega()
{
    return omega;
}




















