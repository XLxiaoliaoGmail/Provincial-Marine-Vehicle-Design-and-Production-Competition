/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-28 21:24:57
 * @LastEditTime: 2023-04-03 23:21:16
 * @FilePath: \MDK-ARMd:\stm32\project\tmp1\Core\User\u_console.h
 */
#ifndef __U_CONSOLE_H
#define __U_CONSOLE_H

#include "main.h"

#define U_CONSOLE_CMD_MAX_LENGTH    64
#define U_CONSOLE_CMD_BEGIN_TAG     '['
#define U_CONSOLE_CMD_END_TAG       ']'
#define U_CONSOLE_CMD_SPLIT_TAG     ' '

typedef struct
{
    // 指令
    char cmd[U_CONSOLE_CMD_MAX_LENGTH];
    // 有效长度
    int length;
    // 是否正在接收
    int is_receiving;
    // 通信串口
    UART_HandleTypeDef *uart;
    // 缓存
    uint8_t buf;
} Console_Typedef;

void Console_Init(UART_HandleTypeDef *uart);
void Console_Log(const char *s, ...);
void Console_Pre_Handler(void);
void Console_Receive_Handler(void);
void Console_Cmd_Handler(void);




#endif
