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
    uart_init(uart1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    char a[3][10]={"CCCCCCCCCC","AAAAAAAAAA","BBBBBBBBBB"};
    while(1){
        for(int g=0; g<3; g++){
            uart_puts(uart1, a[g]);
            printf("Sender send: %s\n", a[g]);
        }
        sleep_ms(1000);
    }
    return 0;
}