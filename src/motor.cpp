#include <Arduino.h>
#include <motor.h>
#include "commen.h"
Motor::Motor(uint8_t _en_pin, uint8_t _input_pin_1, uint8_t _input_pin_2, bool _direction, uint8_t _channel, uint8_t _resolution, int _freq)
{
	en_pin = _en_pin;
	input_pin_1 = _input_pin_1;
	input_pin_2 = _input_pin_2;
	direction = _direction;
	channel = _channel;
	freq = _freq;
	resolution = _resolution;
	max_pwm_value = pow(2,resolution) -1; //最大的PWM数值 = 2的分辨率位数次方 -1   2^8-1 = 255
}

void Motor::init()
{
	// Set all the motor control pins to outputs
	pinMode(en_pin, OUTPUT);
	pinMode(input_pin_1, OUTPUT);
	pinMode(input_pin_2, OUTPUT);

  	ledcSetup(channel, freq, resolution);  //pwm频道, 频率, 精度
  	ledcAttachPin(en_pin, channel);
	// Turn off motor
	digitalWrite(input_pin_1, LOW);
	digitalWrite(input_pin_2, LOW);

  	ledcWrite(channel,0);

}

/**
 * @brief: 设置占空比 -100~+100 正数表示正转，负数表示反转 
 * @param {float} ratio
 * @return {*}
 */
void Motor::run(float ratio)
{
    int pwm_value = max_pwm_value * ratio; //注意这里的PWM数值是带符号的，正数表示正转，负数表示反转 


	if(pwm_value == 0)
	{
		// 停转电机
		digitalWrite(input_pin_1, LOW);
		digitalWrite(input_pin_2, LOW);
	}
	else if ((pwm_value >= 0))
	{
		digitalWrite(input_pin_1, direction);
		digitalWrite(input_pin_2, !direction);
	}
	else
	{
		digitalWrite(input_pin_1, !direction);
		digitalWrite(input_pin_2, direction);
	}
	
  	ledcWrite(channel,_constrain(abs(pwm_value), 0, max_pwm_value));	// 输出PWM 并且限制在 max_pwm_value 之间
}



/**
 * @brief: 刹车
 * @return {*}
 */
void Motor::stop()
{
	// Turn off motor
	digitalWrite(input_pin_1, LOW);
	digitalWrite(input_pin_2, LOW);

  	ledcWrite(channel,0);
}
