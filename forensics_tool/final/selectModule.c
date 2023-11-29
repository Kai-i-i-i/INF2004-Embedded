#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "spifuncs/SPIFUNCS.h"
#include "conversion/conversion.h"

#define SNIFFBUTTON 0
#define UARTBUTTON 1
#define I2CBUTTON 3
#define SPIBUTTON 2
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
}

void init_uart(){
    uart_init(UART_ID, BAUDRATE);
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
    init_spi_pins(SPI_PORT, 500000, SPI0_RX_PIN, SPI0_TX_PIN, SPI0_SCLK_PIN, SPI0_CS_PIN);
    init_spi_pins(SPI1_PORT, 500000, SPI1_RX_PIN, SPI1_TX_PIN, SPI1_SCLK_PIN, SPI1_CS_PIN);
    spi_set_slave(SPI_PORT, true);
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

                //init spi spins


                if (injectState) {
                    //inject

                    uint8_t hardcoded_inject[4];
                    hardcoded_inject[0] = 0x99;
                    hardcoded_inject[1] = 0x12;
                    hardcoded_inject[2] = 0x34;
                    hardcoded_inject[3] = 0x56;

                    //create the buffers for spi
                    uint8_t *write_buffer = create_buffers(4);
                    uint8_t *read_buffer = create_buffers(4);

                    // writing of injected bytes to slave, and relaying appropriate bytes back to master
                    while (1) {
                        inject_spi(write_buffer, read_buffer, hardcoded_inject, 4);

                        // printing of output/logs
                        for (int i = 0; i < 4; i++) {
                            printf("Messaged from spi0: %x\n", read_buffer[i]);
                            printf("Messaged writing to spi1: %x\n", hardcoded_inject[i]);
                            printf("Received from spi1: %x\n", write_buffer[i]);
                            printf("Message write to spi0: %x\n", write_buffer[i]);
                            printf("\n");
                        }
                    }  
                }
                else {
                    //spi sniff

                    int number = 4;
                    int do_sniff = 0;

                    //create buffers for spi
                    uint8_t *write_buffer = create_buffers(number);
                    uint8_t *read_buffer = create_buffers(number);

                    do_sniff = 1;

                    //perform sniffing
                    while (do_sniff == 1) {
                        sniff_spi(write_buffer, read_buffer, number);
                        //printing of output/logs
                        for (int i = 0; i < number; i++) {
                            printf("Message from spi0: %x\n", read_buffer[i]);
                            printf("Message write to spi1: %x\n", read_buffer[i]);
                            printf("Message from spi1: %x\n", write_buffer[i]);
                            printf("Message write to spi0: %x\n", write_buffer[i]);
                            }
                        }
                    }
                break;
            case 0:
                printf("None plugged\n");
                break;
        }
        sleep_ms(200);
    }
    return 0;
}