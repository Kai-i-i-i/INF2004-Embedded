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

// Define the maximum length of strings to be injected or sniffed
const int MAX_STRING_LENGTH = 100;

// Function to test UART1 communication
void test_uart1_rx()
{
  char receivedString[MAX_STRING_LENGTH];
  int receivedStringIndex = 0;

  while (uart_is_readable(UART_ID))
  {
    char receivedChar = uart_getc(UART_ID);

    // Check if the received character is a newline character
    if (receivedChar == '\n')
    {
      // If a newline character is received, print the received string
      printf("Received on PIN9: %s\n", receivedString);

      // Reset the received string index and received string
      receivedStringIndex = 0;
      memset(receivedString, 0, sizeof(receivedString));
    }
    else
    {
      // Append the received character to the received string
      receivedString[receivedStringIndex++] = receivedChar;

      // Check if the received string index reaches the maximum length
      if (receivedStringIndex >= MAX_STRING_LENGTH)
      {
        // If the maximum length is reached, reset the received string index and
        // received string
        receivedStringIndex = 0;
        memset(receivedString, 0, sizeof(receivedString));
      }
    }
  }
  sleep_ms(500);
}

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (1)
    {
        test_uart1_rx();
    }

    return 0;
}