/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-30 09:29:46
 * @LastEditTime: 2023-04-06 21:14:39
 * @FilePath: \MDK-ARMd:\stm32\project\f103c8t6\Core\User\u_motor.c
 */
#include "u_motor.h"

Motor_Typedef motor1;
Motor_Typedef motor2;
/**
 * @description: 初始化电机 电机的CH_Polarity应该为High
 * @return {*} 返回生成的电机对象
 */
Motor_Typedef Motor_Init(TIM_HandleTypeDef *tim_a, uint32_t channel_a, TIM_HandleTypeDef *tim_b, uint32_t channel_b) {
    Motor_Typedef motor;
    motor.channel_a = channel_a;
    motor.channel_b = channel_b;
    motor.tim_a = tim_a;
    motor.tim_b = tim_b;
    HAL_TIM_PWM_Start(tim_a, channel_a);
    __HAL_TIM_SetCompare(tim_a, channel_a, 0);
    HAL_TIM_PWM_Start(tim_b, channel_b);
    __HAL_TIM_SetCompare(tim_b, channel_b, 0);
    return motor;
}
/**
 * @description: 调整电机转速 ina与inb应该是0-100的值 超限会调整至0
 * @param {Motor_Typedef} motor
 * @param {uint8_t} in_a
 * @param {uint8_t} in_b
 * @return {*}
 */
void Motor_Set_Base(Motor_Typedef motor, uint8_t in_a, uint8_t in_b) {
    if (in_a > 100) {
        in_a = 100;
    }
    if (in_b > 100) {
        in_b = 100;
    }
    // 这里用int除法
    __HAL_TIM_SetCompare(motor.tim_a, motor.channel_a, motor.tim_a->Init.Period * in_a / 100);
    __HAL_TIM_SetCompare(motor.tim_b, motor.channel_b, motor.tim_b->Init.Period * in_b / 100);
}
/**
 * @description: 设置转速 speed为0-200 其中0为反转最快 200为正转最快 100为无动力
 * @param {Motor_Typedef} motor
 * @param {uint8_t} speed
 * @return {*}
 */
void Motor_Set(Motor_Typedef motor, int speed) {
    if (speed < 0) {
        Motor_Set_Base(motor, -speed, 0);
    }else if(speed > 0) {
        Motor_Set_Base(motor, 0, speed);
    }else {
        Motor_Set_Base(motor, 0, 0);
    }
}

