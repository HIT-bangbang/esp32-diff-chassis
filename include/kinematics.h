#ifndef KINEMATICS_H
#define KINEMATICS_H

class Kinematics
{
private:

    float width;
    float radius;




public:
    Kinematics(float _width, float _radius);
    void inverse(float cmd_linear, float cmd_angular);
    void forward(float omega_A, float omega_B);

    float get_real_linear_vel();    //运动学正解得到当前真实的线速度
    float get_real_angular_vel();   //运动学正解得到当前真实的角速度

    float target_omega_A = 0.0;  // 逆解得到的期望轮子转速
    float target_omega_B = 0.0;

    float real_linear_vel = 0.0; //运动学正解得到当前真实的线速度角速度
    float real_angular_vel = 0.0;
};


#endif