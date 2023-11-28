#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define SPI_PORT spi0
#define SPI_TX_PIN 19
#define SPI_RX_PIN 16
#define SPI_CS_PIN 17
#define SPI_SCLK_PIN 18

int main() {
    stdio_init_all();
    uint8_t message_buffer[4];
    message_buffer[0] = 0xFF;
    message_buffer[1] = 0x12;
    message_buffer[2] = 0xDE;
    message_buffer[3] = 0xAD;

    uint8_t receiving_buffer[4];

    spi_init(SPI_PORT, 500000);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SPI);

    while (1) {
        spi_write_read_blocking(SPI_PORT, message_buffer, receiving_buffer, 4);
        for (int i = 0; i < 4; i++){
            printf("Data Sent is : %x\n", message_buffer[i]);
            printf("Data Received is %x\n", receiving_buffer[i]);
            
        }
        sleep_ms(1000);
    }
    return 0;
}