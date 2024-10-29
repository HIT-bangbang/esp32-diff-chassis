#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
    bool direction = true;
    uint8_t en_pin;
    uint8_t input_pin_1;
    uint8_t input_pin_2;
    uint8_t resolution;
    int max_pwm_value;
    uint8_t channel;
    int freq;


public:
    Motor(uint8_t _en_pin, uint8_t _input_pin_1,uint8_t _input_pin_2, bool _direction, uint8_t _channel, uint8_t _resolution, int _freq);
    void init();
    void run(float ratio);
    void stop();
};

#endif