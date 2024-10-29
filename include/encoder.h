#ifndef ENCODER_H
#define ENCODER_H
#include <functional>
#include <Arduino.h>

class Encoder
{
private:
    uint8_t enc_A_pin, enc_B_pin;
    int64_t count = 0;         // 编码器脉冲计数，范围为(0~2^64-1)，不太可能会溢出
    int64_t last_count;    // 上一次速度更新时，编码器计数 

    int8_t direction;

    unsigned long last_time; // 上一次更新速度的时间

    float omega;
    int enc_res;    //轮子转一圈有多少脉冲 = 减速比*编码器分辨率

    std::function<void(void)> Int_Callback_A;
    std::function<void(void)> Int_Callback_B;

public:
    Encoder(uint8_t _enc_A_pin, uint8_t _enc_B_pin, int8_t _direction, int _enc_res);
    void init();
    void Callback_A();
    void Callback_B();
    void update();          // 更新轮子角速度
    float get_omega();      // 获取角速度
    int64_t get_count();   // 获取计数值
};

#endif