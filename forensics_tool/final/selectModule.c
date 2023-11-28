#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#define SNIFFBUTTON 0
#define UARTBUTTON 1
#define I2CBUTTON 2
#define SPIBUTTON 3
#define RXBUTTON 9
#define TXBUTTON 8
#define UART_ID uart1
#define BAUDRATE 115200

uint32_t lastDebounce = 0;
int state = 0;
int injectState = 0;
int uartInitFlag = 0;

void init(){
    stdio_init_all();
    for(int i=0; i<4; i++){
        gpio_init(i);
        gpio_set_dir(i, 0);
        gpio_set_pulls(i,1,0);
    }
    // gpio_init(SNIFFBUTTON);
    // gpio_init(UARTBUTTON);
    // gpio_init(I2CBUTTON);
    // gpio_init(SPIBUTTON);
    // gpio_set_dir(SNIFFBUTTON, GPIO_IN);
    // gpio_set_dir(UARTBUTTON, GPIO_IN);
    // gpio_set_dir(I2CBUTTON, GPIO_IN);
    // gpio_set_dir(SPIBUTTON, GPIO_IN);
    // gpio_set_pulls(SNIFFBUTTON, true, false);
    // gpio_set_pulls(UARTBUTTON, true, false);
    // gpio_set_pulls(I2CBUTTON, true, false);
    // gpio_set_pulls(SPIBUTTON, true, false);
}

void init_uart(){
    uart_init(UART_ID, BAUDRATE);
    // uart_set_fifo_enabled(UART_ID, false);
    gpio_set_function(RXBUTTON, GPIO_FUNC_UART);
    gpio_set_function(TXBUTTON, GPIO_FUNC_UART);
    uartInitFlag=1;
}

bool debounce(bool state){
    uint32_t currentTime = to_ms_since_boot(get_absolute_time());
    if(currentTime-lastDebounce>=300){
        lastDebounce=currentTime;
        return state;
    }
    return state;
}

int getState(){
    // state = gpio_get(UARTBUTTON);
    // uartState = (state==0) ? 1 : 0;
    injectState = (gpio_get(SNIFFBUTTON)) ? 0: 1; 
    int uartState = (gpio_get(UARTBUTTON)) ? 0 : 2;
    int i2cState = (gpio_get(I2CBUTTON)) ? 0 : 4;
    int spiState = (gpio_get(SPIBUTTON)) ? 0 : 8;
    state = uartState+i2cState+spiState;
    return state;
}

void uartMode(){
    char readChar;
    if(!uartInitFlag){
        init_uart();
    }
    while(true){
        if(injectState){
            char payload[8]="DEADBEEF";
            printf("INJECTING: %s\n",payload);
            uart_puts(UART_ID,payload);
            if(uart_is_readable(UART_ID)){
                readChar = uart_getc(UART_ID);
                printf("Sniffer receive: %c\n", readChar);
                printf("Sniffer send: %c\n", readChar);
                uart_putc(UART_ID, readChar);
            }
            sleep_ms(1000);
        }
        else{
            if(uart_is_readable(UART_ID)){
                readChar = uart_getc(UART_ID);
                printf("Sniffer receive: %c\n", readChar);
                printf("Sniffer send: %c\n", readChar);
                uart_putc(UART_ID, readChar);
            }
        }
        if(!getState()){
            return;
        }
    }
}


int main(){
    init();
    while(true){
        int state = getState();
        switch(state){
            case 2:
                uartMode();
                break;
            case 4:
                printf("I2C ");
                break;
            case 8:
                printf("SPI ");
                break;
            case 0:
                printf("None plugged\n");
                break;
        }
        // printf("%d",state);
        sleep_ms(200);
    }
    // gpio_set_irq_enabled_with_callback(UARTBUTTON, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &uart_irq_handler);
    // for(;;){
    //     sleep_ms(UINT32_MAX);
    // }
    return 0;
}