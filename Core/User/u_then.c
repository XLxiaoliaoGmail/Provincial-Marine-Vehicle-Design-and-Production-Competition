/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-28 23:02:04
 * @LastEditTime: 2023-04-01 21:53:43
 * @FilePath: \MDK-ARMd:\stm32\project\tmp1\Core\User\u_then.c
 */
#include "u_then.h"

Then_Typedef then;

/**
 * @description: 初始化 在完成功能前必须要调用此函数
 * @param {TIM_HandleTypeDef} *htim
 * @return {*}
 */
void Then_Init(TIM_HandleTypeDef *htim)
{
    then.tim = htim;
    then.is_working = 0;
}
/**
 * @description: 查找一个空闲的工作模块id 此操作不更新is_full
 * @return {*} 空闲的线路的最小id 若无则返回-1
 */

int Then_Find_Free(void)
{
    int index = -1;
    int i = 0;
    for (; i<U_THEN_MAX_WORK_NUM; i++)
    {
        if (!then.count[i])
        {
            index = i;
            break;
        }        
    }
    return index;
}
/**
 * @description: 
 * @param {int} ms
 * @return {*}
 */
int Then_Work(int ms, void(*funct)(void))
{
    int index;
    index = Then_Find_Free();
    if (index == -1)
    {
        return -1;
    }
    then.count[index] = ms;
    then.u_then_function[index] = funct;
    then.is_working = 1;
    HAL_TIM_Base_Start_IT(then.tim);
    return index;
}

void Then_Interrupt_Handler(void) {
    // 标记是否有计数器在工作状态
    int flag = 0;
    // 将所有工作中的线路count减一
    for(int i=0; i<U_THEN_MAX_WORK_NUM; i++)
    {
      if(then.count[i])
      {
        then.count[i] --;
        flag = 1;
        if(!then.count[i])
        {
          // 若减完后变为0 则触发函数
          (*then.u_then_function[i])();
        }
      }
    }
    if(!flag)
    {
      // 若都没有工作中的通道 则关闭定时器
      HAL_TIM_Base_Stop_IT(then.tim);
    }
}

/* END */
