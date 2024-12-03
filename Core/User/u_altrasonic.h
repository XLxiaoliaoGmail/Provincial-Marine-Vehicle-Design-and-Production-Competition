#ifndef __U_ALTRASONIC_H
#define __U_ALTRASONIC_H

#include "main.h"

typedef struct {
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    TIM_HandleTypeDef *tim;
    int count;
    uint8_t is_counting;
} Altrasonic_Typedef;

void Astrasonic_Init(GPIO_TypeDef *trig_port, uint16_t trig_pin, GPIO_TypeDef *echo_port, uint16_t echo_pin, TIM_HandleTypeDef *tim);
void Astrasonic_Set_Trig_Low(void);
void Astrasonic_Collect(void);
void Altrasonic_Start_Count(void);
void Altrasonic_Stop_Count(void);
void Altrasonic_Tim_Handler(void);
void Altrasonic_Finish(void);

#endif
