#include <Arduino.h>
#include "commen.h"
#include "motor.h"
#include "encoder.h"
#include "kinematics.h"
#include "pid.h"

#include <ros.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int64MultiArray.h>
#include <std_msgs/Float32MultiArray.h>


void cmd_vel_cb(const geometry_msgs::Twist& cmd_vel);
void pub_wheelticks();
void pub_wheelomegas();


ros::NodeHandle nh;

std_msgs::Int64MultiArray counters;  // 发布 编码器计数值
std_msgs::Float32MultiArray omegas;   // 发布两个车轮的角速度



// ! Subscriber
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &cmd_vel_cb);

// ! Publisher
ros::Publisher pub_encoders("wheel_ticks", &counters);
ros::Publisher pub_omegas("wheel_omegas", &omegas);


// ! TF
//geometry_msgs::TransformStamped TF_odo2base;  // TF话题消息
//tf::TransformBroadcaster broadcaster;         // TF广播器
//char base_link[] = "/base_link";
//char odom[] = "/odom";

unsigned long time_last = 0;  // 记录时间，单位ms
unsigned long time_cur = 0;

Motor motor_A(MA_ENB,MA_IN1,MA_IN2,MA_DIR,MA_PWM_CHANNEL, RESOLUTION, FREQ);
Motor motor_B(MB_ENB,MB_IN1,MB_IN2,MB_DIR,MB_PWM_CHANNEL, RESOLUTION, FREQ);

Encoder encoder_A(MA_ENC_A, MA_ENC_B, MA_ENC_DIR, ENC_RES);
Encoder encoder_B(MB_ENC_A, MB_ENC_B, MB_ENC_DIR, ENC_RES);

Kinematics kinematics(WIDTH, RADIUS);

PIDController mA_PID(KP,KI,KD,RAMP,PID_LIMIT);
PIDController mB_PID(KP,KI,KD,RAMP,PID_LIMIT);

float target_omega_A = 10.0;
float target_omega_B = 10.0;

float Duty_ratio_A = 0.0;
float Duty_ratio_B = 0.0;

void setup()
{
  // 主频拉满240Mhz
  //setCpuFrequencyMhz(240); 

  //setBaud(); 

  nh.initNode();
  nh.advertise(pub_encoders);
  nh.advertise(pub_omegas);

  nh.subscribe(sub);

  // TF 广播器初始化
  //broadcaster.init(nh); 
  
  //Serial.begin(115200);
  //Serial.println(ets_get_cpu_frequency());

  // 电机初始化
  motor_A.init();
  motor_A.stop();
  motor_B.init();
  motor_B.stop();

  // 编码器初始化
  encoder_A.init();
  encoder_B.init();

}

void loop()
{

  // * 更新轮子实际角速度
  encoder_A.update();
  encoder_B.update();

  // * 运动学正解 omega A B --> 线速度 角速度
  kinematics.forward(encoder_A.get_omega(),encoder_B.get_omega());

  // * PID 两个轮的omega(期望 - 实际) ---->  两个电机的占空比
  // Duty_ratio_A = mA_PID(target_omega_A - encoder_A.get_omega());
  // Duty_ratio_B = mB_PID(target_omega_B - encoder_B.get_omega());

  Duty_ratio_A = mA_PID(kinematics.target_omega_A - encoder_A.get_omega());
  Duty_ratio_B = mB_PID(kinematics.target_omega_B - encoder_B.get_omega());

  // * 输出到电机
  //motor_A.stop();
  motor_A.run(Duty_ratio_A);
  //motor_B.stop();
  motor_B.run(Duty_ratio_B);

  // Serial.print("omegaA: ");
  // Serial.println(encoder_A.get_omega());

  pub_wheelticks();
  pub_wheelomegas();

  nh.spinOnce();
  delay(100);
}

// ! ros cmd_vel 回调函数
void cmd_vel_cb(const geometry_msgs::Twist& cmd_vel)
{
  // * 运动学逆解
  kinematics.inverse(cmd_vel.linear.x,cmd_vel.angular.z);
}

void pub_wheelticks()
{
  int64_t wheel_ticks[2] = {encoder_A.get_count(),encoder_B.get_count()};

  counters.data_length = 2;     // 数据长度为 2
  counters.data = wheel_ticks;  

  pub_encoders.publish(&counters); // 回传两个编码器计数值
}

void pub_wheelomegas()
{
  float wheel_omegas[2] = {encoder_A.get_omega(),encoder_B.get_omega()};

  omegas.data_length = 2;     // 数据长度为 2
  omegas.data = wheel_omegas;  

  pub_omegas.publish(&omegas); // 回传两个编码器计数值
}



// 发布  TF(odom --> base_link)
// void pub_TF()
// {
//   TF_odo2base.header.frame_id = odom;
//   TF_odo2base.child_frame_id = base_link;
//   TF_odo2base.transform.translation.x = 1.0;
//   TF_odo2base.transform.rotation.x = 0.0;
//   TF_odo2base.transform.rotation.y = 0.0;
//   TF_odo2base.transform.rotation.z = 0.0;
//   TF_odo2base.transform.rotation.w = 1.0;
//   TF_odo2base.header.stamp = nh.now();    // 时间戳，从PC端获取的，与PC端时间对齐
//   broadcaster.sendTransform(TF_odo2base); // 发布  TF
// }
