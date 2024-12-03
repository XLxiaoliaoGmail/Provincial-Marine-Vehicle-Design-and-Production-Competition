#include "u_altrasonic.h"

#include "u_then.h"
#include "u_console.h"

Altrasonic_Typedef altrasonic;

void Astrasonic_Init(
    GPIO_TypeDef *trig_port,
    uint16_t trig_pin,
    GPIO_TypeDef *echo_port,
    uint16_t echo_pin,
    TIM_HandleTypeDef *tim
) {
    altrasonic.trig_port = trig_port;
    altrasonic.trig_pin = trig_pin;
    altrasonic.echo_port = echo_port;
    altrasonic.echo_pin = echo_pin;
    altrasonic.tim = tim;
    altrasonic.count = 0;
    altrasonic.is_counting = 0;
}

void Astrasonic_Set_Trig_Low(void) {
    HAL_GPIO_WritePin(altrasonic.trig_port, altrasonic.trig_pin, GPIO_PIN_RESET);
}

void Astrasonic_Sharp(void) {
    if (!altrasonic.is_counting) {
        return;
    }
    altrasonic.count = 0;
    altrasonic.is_counting = 0;
    Console_Log("Astrasonic Sharp ...");
}

void Astrasonic_Collect(void) {
    // trig拉高1ms 这里默认then.c的时钟周期是1ms
    HAL_GPIO_WritePin(altrasonic.trig_port, altrasonic.trig_pin, GPIO_PIN_SET);
    Then_Work(1, Astrasonic_Set_Trig_Low);
    altrasonic.count = 0;
    altrasonic.is_counting = 011;
    // 倘若无响应 则在100ms之后自动停止
    Then_Work(100, Astrasonic_Sharp);
}

void Altrasonic_Start_Count(void) {
    // 开启定时器中断 每10us中断一次
    HAL_TIM_Base_Start_IT(altrasonic.tim);
}

void Altrasonic_Stop_Count(void) {
    altrasonic.is_counting = 0;
}

void Altrasonic_Tim_Handler(void) {
    altrasonic.count ++;
    if (!altrasonic.is_counting) {
        HAL_TIM_Base_Stop_IT(altrasonic.tim);
        Altrasonic_Finish();
    }
}

void Altrasonic_Finish(void) {
    Console_Log("count = %d", altrasonic.count);
}
