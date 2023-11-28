#ifndef SPIFUNCS_H_
#define SPIFUNCS_H_

#include "hardware/spi.h"
#include "pico/stdlib.h"

#define SPI0_RX_PIN 16
#define SPI0_TX_PIN 19
#define SPI0_SCLK_PIN 18
#define SPI0_CS_PIN 17
#define SPI_PORT spi0

#define SPI1_RX_PIN 12
#define SPI1_TX_PIN 11
#define SPI1_SCLK_PIN 10
#define SPI1_CS_PIN 13
#define SPI1_PORT spi1

uint8_t *create_buffers(int number_of_bytes);

void sniff_spi(uint8_t *write_buffer, uint8_t *read_buffer, int number_of_bytes);

void inject_spi(uint8_t *write_buffer, uint8_t *read_buffer, uint8_t *inject_buffer, size_t number_of_bytes);

void init_spi_pins(spi_inst_t* spi, uint baudrate, uint spi_rx, uint spi_tx, uint spi_sclk, uint spi_cs);

#endif //SPIFUNCS_H_