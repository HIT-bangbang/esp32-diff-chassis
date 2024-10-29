#include <Arduino.h>
#include <motor.h>
#include "commen.h"
Motor::Motor(uint8_t _en_pin, uint8_t _input_pin_1, uint8_t _input_pin_2, bool _direction, uint8_t _channel_1,uint8_t _channel_2, uint8_t _resolution, int _freq)
{
	en_pin = _en_pin;
	input_pin_1 = _input_pin_1;
	input_pin_2 = _input_pin_2;
	direction = _direction;
	channel_1 = _channel_1;
	channel_2 = _channel_2;

	freq = _freq;
	resolution = _resolution;
	max_pwm_value = pow(2,resolution) -1-3; //最大的PWM数值 = 2的分辨率位数次方 -1   2^8-1 = 255  //! 而且要避免上管一直开，下管一直管的状态，所以留出 3 的余量
}

void Motor::init()
{
	// Set all the motor control pins to outputs
	pinMode(en_pin, OUTPUT);
	pinMode(input_pin_1, OUTPUT);
	pinMode(input_pin_2, OUTPUT);

  	ledcSetup(channel_1, freq, resolution);  //pwm频道, 频率, 精度
  	ledcSetup(channel_2, freq, resolution);  //pwm频道, 频率, 精度

  	ledcAttachPin(input_pin_1, channel_1);
  	ledcAttachPin(input_pin_2, channel_2);

	// Enable EG2104 SD
	digitalWrite(en_pin, HIGH);

	// eg2104 的SD非常离谱，SD拉高之后，不能直接控制LO，必须要IN给一次高电平之后，才能控制LO。
	// IR2104 就不用。按道理讲，控制LO根本不需要自举电路啊，搞不懂EG2104为啥非要IN给高才能使能
  	ledcWrite(channel_1,max_pwm_value);
  	ledcWrite(channel_2,max_pwm_value);
	delay(10);
	// 关闭电机，两个低管导通，电机两相直连，处于拖曳制动状态
  	ledcWrite(channel_1,0);
  	ledcWrite(channel_2,0);

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
		// 停转电机。AB相同时上管关闭，下管导通，制动
		ledcWrite(channel_1,0);
		ledcWrite(channel_2,0);
	}
	else if ((pwm_value >= 0))
	{
  		ledcWrite(channel_1,0);		// A相下管导通，上管关闭
  		ledcWrite(channel_2,_constrain(abs(pwm_value), 0, max_pwm_value));	// B相 输出PWM 并且限制在 max_pwm_value 之间
	}
	else
	{
  		ledcWrite(channel_1,_constrain(abs(pwm_value), 0, max_pwm_value));	// A相 输出PWM 并且限制在 max_pwm_value 之间
  		ledcWrite(channel_2,0);		// B相下管导通，上管关闭
	}
	
}

/**
 * @brief: 刹车
 * @return {*}
 */
void Motor::stop()
{
	// Stop motor
  	ledcWrite(channel_1,0);
  	ledcWrite(channel_2,0);
}
