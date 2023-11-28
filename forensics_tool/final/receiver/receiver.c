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

int main(){
    stdio_init_all();
    char readChar;
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    while(1){
        if(uart_is_readable(UART_ID)){
            readChar = uart_getc(UART_ID);
            printf("Receiver receive: %c\n", readChar);
        }
    }
    return 0;
}