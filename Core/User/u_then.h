/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-28 23:02:11
 * @LastEditTime: 2023-04-01 21:53:50
 * @FilePath: \MDK-ARMd:\stm32\project\tmp1\Core\User\u_then.h
 */
#ifndef __U_THEN_H
#define __U_THEN_H

#include "main.h"

#define U_THEN_MAX_WORK_NUM 8

typedef struct
{
    // 各线路的工作计数 向下计数 计数到0时触发
    int count[U_THEN_MAX_WORK_NUM];
    // 使用的计时器
    TIM_HandleTypeDef *tim;
    // 回调函数
    void(*u_then_function[U_THEN_MAX_WORK_NUM])(void);
    // 是否有至少一条线路在工作
    int is_working;
} Then_Typedef;

void Then_Init(TIM_HandleTypeDef *htim);
int Then_Find_Free(void);
int Then_Work(int ms, void(*funct)(void));
void Then_Interrupt_Handler(void);



#endif
