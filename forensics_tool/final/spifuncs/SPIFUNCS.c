#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPIFUNCS.h"

void init_spi_pins(spi_inst_t* spi, uint baudrate, uint spi_rx, uint spi_tx, uint spi_sclk, uint spi_cs) {
    spi_init(spi, baudrate); //initialize spi0 at 500000kHz

    gpio_set_function(spi_rx, GPIO_FUNC_SPI);
    gpio_set_function(spi_tx, GPIO_FUNC_SPI);
    gpio_set_function(spi_sclk, GPIO_FUNC_SPI);
    gpio_set_function(spi_cs, GPIO_FUNC_SPI);

}

void sniff_spi(uint8_t *write_buffer, uint8_t *read_buffer, int number_of_bytes) {

    spi_write_read_blocking(SPI_PORT, write_buffer, read_buffer, number_of_bytes);
    spi_write_read_blocking(SPI1_PORT, read_buffer, write_buffer, number_of_bytes);
    

}

void inject_spi(uint8_t *write_buffer, uint8_t *read_buffer, uint8_t *inject_buffer, size_t number_of_bytes) {

    spi_write_read_blocking(SPI_PORT, write_buffer, read_buffer, number_of_bytes);
    spi_write_read_blocking(SPI1_PORT, inject_buffer, write_buffer, number_of_bytes);
    
}

uint8_t *create_buffers(int number_of_bytes) {
    uint8_t *temp_buffer;
    temp_buffer = malloc(sizeof(uint8_t) * number_of_bytes);
    memset(temp_buffer, 0, sizeof(uint8_t) * number_of_bytes);

    return temp_buffer;
}

