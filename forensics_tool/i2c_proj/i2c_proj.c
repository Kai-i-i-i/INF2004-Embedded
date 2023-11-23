#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C_ADDRESS 0x08  // Choose an I2C address not conflicting with other devices

int main() {
    stdio_init_all();

    #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
        #warning i2c/self_communication example requires a board with I2C pins
        puts("Default I2C pins were not defined");
    #else
        i2c_init(i2c_default, 100 * 1000);
        gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
        gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
        bi_decl(bi_2pins_with_func(4, 5, GPIO_FUNC_I2C));

        // Set up I2C slave address
        uint8_t i2c_address = I2C_ADDRESS;

        // Infinite loop for continuous communication
        while (1) {
            // Send a message to itself
            uint8_t tx_data[] = "Hello, Pico!";
            i2c_write_blocking(i2c_default, i2c_address, tx_data, sizeof(tx_data) - 1, false);

            // Wait for a moment
            sleep_ms(1000);

            // Read the message from itself
            uint8_t rx_data[16];  // Adjust the buffer size as needed
            i2c_read_blocking(i2c_default, i2c_address, rx_data, sizeof(rx_data) - 1, false);

            rx_data[sizeof(rx_data) - 1] = '\0';

            // Print the received message to the serial monitor
            printf("Received message: %s\n", rx_data);

            // Wait for a moment before sending the next message
            sleep_ms(1000);
        }

        return 0;
    #endif
}