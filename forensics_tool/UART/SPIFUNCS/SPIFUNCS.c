#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "SPIFUNCS.h"



int read_from_spi(spi_inst_t* spi, uint8_t message_buffer[4]) {

    int return_value = 0;
    //gpio_put(SPI0_CS_PIN, 0);
    return_value = spi_read_blocking(SPI_PORT, 0, message_buffer, 4);
    //gpio_put(SPI0_CS_PIN, 1);

    return return_value;
    
}

int write_to_spi(spi_inst_t* spi, uint8_t message_buffer[4]) {

    int return_value = 0;
    gpio_put(SPI1_CS_PIN, 0);
    return_value = spi_write_blocking(SPI1_PORT, message_buffer, 4);
    gpio_put(SPI1_CS_PIN, 1);

    return return_value;
}

void init_spi_pins(spi_inst_t* spi, uint baudrate, uint spi_rx, uint spi_tx, uint spi_sclk, uint spi_cs) {
    spi_init(spi, baudrate); //initialize spi0 at 500000kHz

    gpio_set_function(spi_rx, GPIO_FUNC_SPI);
    gpio_set_function(spi_tx, GPIO_FUNC_SPI);
    gpio_set_function(spi_sclk, GPIO_FUNC_SPI);
    gpio_set_function(spi_cs, GPIO_FUNC_SPI);

}