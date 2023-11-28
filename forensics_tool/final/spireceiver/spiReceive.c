#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hardware/spi.h" 
 
 
#define SPI_RX_PIN 12 
#define SPI_TX_PIN 11 
#define SPI_CS_PIN 13 
#define SPI_SCLK_PIN 10 
#define SPI_PORT spi1
 
int main() { 
 
    stdio_init_all(); 
    spi_init(SPI_PORT, 500000); 
    spi_set_slave(SPI_PORT, true); 
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI); 
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI); 
    gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SPI); 
    gpio_set_function(SPI_SCLK_PIN, GPIO_FUNC_SPI); 
 
    uint8_t input_buffer[4], output_buffer[4]; 
 
    output_buffer[0] = 0xDE; 
    output_buffer[1] = 0xAD; 
    output_buffer[2] = 0xBE; 
    output_buffer[3] = 0xEF; 
    /*
    input_buffer[0] = 0x01; 
    input_buffer[1] = 0x23; 
    input_buffer[2] = 0x45; 
    input_buffer[4] = 0x67; 
    */
    while (1) { 
        spi_write_read_blocking(SPI_PORT, output_buffer, input_buffer, 4); 
        if (input_buffer[0] == 0x99) { 
            output_buffer[0] = 0x69; 
        } 
 
        for (int i = 0; i < 4; i++) { 
            printf("message received: %x\n", input_buffer[i]); 
            printf("message sent: %x\n", output_buffer[i]); 
        } 
        sleep_ms(1000);
    } 
    return 0; 
}