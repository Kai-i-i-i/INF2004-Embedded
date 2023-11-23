#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN 8
#define UART_RX_PIN 9

// Function to test UART1 communication with second pico
void test_uart1_tx()
{
    char *testString = "Hello from Pin 8!\n";
    uart_puts(UART_ID, testString);
    sleep_ms(200);
}

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (1)
    {
        test_uart1_tx();
    }

    return 0;
}