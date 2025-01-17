#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "tracked_chassis_control.h"

static const char *TAG = "tracked_chassis_control";

#define ECHO_UART_PORT_NUM      (1)
#define ECHO_UART_BAUD_RATE     (115200)
#define BUF_SIZE                (1024)

static void echo_uart_init(void)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, UART_ECHO_TXD, UART_ECHO_RXD, UART_ECHO_RTS, UART_ECHO_CTS));
}

void tracked_chassis_control_start(void)
{
    ESP_LOGI(TAG, "The tracked chassis can now be controlled.");
    echo_uart_init();
}

void tracked_chassis_motion_control(const char* command_str)
{
    uint8_t len = strlen(command_str);

    uart_write_bytes(ECHO_UART_PORT_NUM, command_str, len);

    ESP_LOGI(TAG, "Sent command: %s", command_str);
}


void tracked_chassis_rgb_light_control(uint8_t rgb_mode)
{
    char command = 'w';

    ESP_LOGI(TAG, "command: %c, rgb mode: %d", command, rgb_mode);
    char data[10] = {0};
    sprintf(data, "%c%d", command, rgb_mode);
    uint8_t len = strlen(data);
    uart_write_bytes(ECHO_UART_PORT_NUM, data, len);
}

void tracked_chassis_set_dance_mode(uint8_t dance_mode)
{
    char command = 'd';

    ESP_LOGI(TAG, "command: %c, dance mode: %d", command, dance_mode);
    char data[10] = {0};
    sprintf(data, "%c%d", command, dance_mode);
    uint8_t len = strlen(data);
    uart_write_bytes(ECHO_UART_PORT_NUM, data, len);
}

