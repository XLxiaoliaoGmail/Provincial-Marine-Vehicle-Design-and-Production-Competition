/*
 * @Author: XLxiaoliao
 * @Date: 2023-03-29 22:22:31
 * @LastEditTime: 2023-04-03 23:14:20
 * @FilePath: \MDK-ARMd:\stm32\project\tmp1\Core\User\u_bluetooth.c
 */
#include "u_bluetooth.h"

Bluetooth_Typedef bluetooth;

void Bluetooth_Init(UART_HandleTypeDef *uart) {
    bluetooth.uart = uart;
}

