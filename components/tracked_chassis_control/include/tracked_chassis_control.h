#pragma once

#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UART_ECHO_TXD (GPIO_NUM_38)
#define UART_ECHO_RXD (GPIO_NUM_48)
#define UART_ECHO_RTS (-1)
#define UART_ECHO_CTS (-1)

#define MOTOR_SPEED_MAX 100
#define MOTOR_SPEED_80  80
#define MOTOR_SPEED_60  60
#define MOTOR_SPEED_MIN 0

void tracked_chassis_control_start(void);
void tracked_chassis_motion_control(const char* command_str);
void tracked_chassis_rgb_light_control(uint8_t rgb_mode);
void tracked_chassis_set_dance_mode(uint8_t dance_mode);

#ifdef __cplusplus
}
#endif

