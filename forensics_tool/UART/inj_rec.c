#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "menu/menu.h"
#include "SPIFUNCS/SPIFUNCS.h"
#include "conversion/conversion.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// Setting the pins for TX and RX.
#define UART_TX_PIN 16
#define UART_RX_PIN 17

#define UART_TX_PIN_TESTING 8
#define UART_RX_PIN_TESTING 9

// Setting pin 15 as a pseudo button.
const uint GP15_PIN = 15;

// Define the maximum length of strings to be injected or sniffed
const int MAX_STRING_LENGTH = 100;

// Declare default string to be sent. (To be changed to dynamic later on)
char *injectString = "Hello from Pico!\n";

// Function to send data based on GP15's state
void inject() {
  // When GP15 is high, send the string 'Hello from Pico!' through UART0.
  uart_puts(UART_ID, injectString);
  sleep_ms(200);  // Add a small delay after sending the string for consistency
}

void sniff() {
  // When GP15 is low, enter sniffing mode.
  char receivedString[MAX_STRING_LENGTH];
  int receivedStringIndex = 0;

  while (uart_is_readable(UART_ID)) {
    char receivedChar = uart_getc(UART_ID);

    // Check if the received character is a newline character
    if (receivedChar == '\n') {
      // If a newline character is received, print the received string
      printf("Received string: %s\n", receivedString);

      // Added line to "forward" recieved message out after printing
      uart_puts(uart1, receivedString);

      // Reset the received string index and received string
      receivedStringIndex = 0;
      memset(receivedString, 0, sizeof(receivedString));
    } else {
      // Append the received character to the received string
      receivedString[receivedStringIndex++] = receivedChar;

      // Check if the received string index reaches the maximum length
      if (receivedStringIndex >= MAX_STRING_LENGTH) {
        // If the maximum length is reached, reset the received string index
        // and received string
        receivedStringIndex = 0;
        memset(receivedString, 0, sizeof(receivedString));
      }
    }
  }
}

//test test

// Function to test UART1 communication
void test_uart1_tx() {
    char *testString = "Hello from Pin 8!\n";
    uart_puts(uart1, testString); // Change back to UART0 later
    sleep_ms(200);
}

// Function to test UART1 communication
void test_uart1_rx() {
  char receivedString[MAX_STRING_LENGTH];
  int receivedStringIndex = 0;

  while (uart_is_readable(uart1)) {
    char receivedChar = uart_getc(uart1);

    // Check if the received character is a newline character
    if (receivedChar == '\n') {
      // If a newline character is received, print the received string
      printf("Received from PIN9: %s\n", receivedString);

      // Reset the received string index and received string
      receivedStringIndex = 0;
      memset(receivedString, 0, sizeof(receivedString));
    } else {
      // Append the received character to the received string
      receivedString[receivedStringIndex++] = receivedChar;

      // Check if the received string index reaches the maximum length
      if (receivedStringIndex >= MAX_STRING_LENGTH) {
        // If the maximum length is reached, reset the received string index and
        // received string
        receivedStringIndex = 0;
        memset(receivedString, 0, sizeof(receivedString));
      }
    }
  }
  sleep_ms(500);
}

int main() {
  // Initialize.
  stdio_init_all();

  //menu tracker things
  char menu_buffer[1024];
  int in_menu = 0;
  int menu_activated = 1;
  
  while (true) {
    // test_uart1_tx(); // Call the function to test UART1 communication (TX)
    // test_uart1_rx(); // Call the function to test UART1 communication (RX)
    // send_data();     // Call the function to send or receive data based on
    // GP15's state sleep_ms(1000);   // Wait for 1 second between cycles

    if (scanf("%s", menu_buffer)) {
      menu_activated = 0;
    }
    if (menu_activated == 0 && in_menu == 0) {
      print_menu();
      scanf("%s", menu_buffer);
      //printf("menubuffer= %s\n", menu_buffer);
      //printf("strcmp return: %d", strcmp(menu_buffer, "3"));
      in_menu = 1;
      if (strcmp(menu_buffer, "2") == 0) {
        //selected UART
        printf("uart selected");
         // Initialize UART0 (uart0)
        uart_init(UART_ID, BAUD_RATE);
      //   uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

        // Initialize UART1 (uart1) for testing
        uart_init(uart1, BAUD_RATE);
      //   uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
        gpio_set_function(UART_TX_PIN_TESTING, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN_TESTING, GPIO_FUNC_UART);

        gpio_init(GP15_PIN);

        // Set GP15 (GPIO15) as an input pin to read its state.
        gpio_set_dir(GP15_PIN, GPIO_IN);
        gpio_set_pulls(GP15_PIN, true, false);
        
        while (1) {
        if (gpio_get(GP15_PIN)) {
          inject();
          sleep_ms(10);
          test_uart1_rx(); // Call the function to test UART1 communication (RX) only when GP15 is high
        }
        else {
          test_uart1_tx(); // Call the function to test UART1 communication (TX) only when GP15 is low
          sleep_ms(10);
          sniff();
        }
        sleep_ms(500); // Wait for 0.5 seconds between cycles
        }
      }
      
      if (strcmp(menu_buffer, "3") == 0) {
        int spi_menu = 0;
        uint8_t message_buffer[1024];
        uint8_t spi0_input_buffer[1024];
        int in_spi_sniff = 0;
        int in_spi_inject = 0;

        init_spi_pins(SPI_PORT, 500000, SPI0_RX_PIN, SPI0_TX_PIN, SPI0_SCLK_PIN, SPI0_CS_PIN);
        init_spi_pins(SPI1_PORT, 500000, SPI1_RX_PIN, SPI1_TX_PIN, SPI1_SCLK_PIN, SPI1_CS_PIN);
        spi_set_slave(SPI_PORT, true);

        printf("1. Sniff");
        printf("\n2. Inject");
        scanf("%d", &spi_menu);

        if (spi_menu == 1) {
          while (in_spi_sniff != 1) {
            spi_write_read_blocking(SPI_PORT, spi0_input_buffer,message_buffer, 4); //read from master on left, write to master on left
            //uint8_t relay_buffer[4];
            //sleep_ms(1000);
            /*
            for (int i = 0; i < 4; i++) {
              relay_buffer[i] = message_buffer[i];
            }
            */
            spi_write_read_blocking(SPI1_PORT, message_buffer, spi0_input_buffer, 4); //write to slave on right, read from slave on right
            //sleep_ms(1000);
            for (int i = 0; i < 4; i++) {
              printf("message from spi: %x\n", message_buffer[i]);
              printf("message writing to spi: %x\n", message_buffer[i]);
              printf("\n");
            }
          }
        }
        if (spi_menu == 2) {
          uint8_t injected_input[1024];
          char user_hex_input[1024];
          printf("Enter bytes to send (eg. ff00112233): ");
          scanf("%s", user_hex_input);
          int input_len = count_char(user_hex_input);
          size_t hexa = convert_hex(injected_input, input_len, user_hex_input);

          while (in_spi_inject != 1) {
              //spi_write_read_blocking(SPI1_PORT, message_buffer, )
              spi_write_read_blocking(SPI_PORT, spi0_input_buffer, message_buffer, hexa); //read from master on left, write to master on left
              spi_write_read_blocking(SPI1_PORT, injected_input, spi0_input_buffer, hexa); //write to slave with injected input on right, read from slave on right
              for (int i = 0; i < 4; i++) {
                printf("messaged from spi1: %x\n", message_buffer[i]);
                printf("messaged writing to spi1: %x\n", injected_input[i]);
                printf("received from ruizhe: %x\n", spi0_input_buffer[i]);
                printf("\n");
              }
          }
        }
      }
    }

  }

  return 0;
}
