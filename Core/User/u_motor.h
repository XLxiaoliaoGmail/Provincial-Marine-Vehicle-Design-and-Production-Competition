/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-30 09:29:46
 * @LastEditTime: 2023-03-30 20:56:54
 * @FilePath: \MDK-ARMd:\stm32\project\tmp1\Core\User\u_motor.h
 */
#ifndef __U_MOTOR_H
#define __U_MOTOR_H

#include "main.h"

typedef struct
{
    TIM_HandleTypeDef *tim_a;
    uint32_t channel_a;
    TIM_HandleTypeDef *tim_b;
    uint32_t channel_b;
    uint8_t in_a;
    uint8_t in_b;    
}Motor_Typedef;

Motor_Typedef Motor_Init(TIM_HandleTypeDef *tim_a, uint32_t channel_a, TIM_HandleTypeDef *tim_b, uint32_t channel_b);
void Motor_Set_Base(Motor_Typedef motor, uint8_t in_a, uint8_t in_b);
void Motor_Set(Motor_Typedef motor, int speed);


#endif
