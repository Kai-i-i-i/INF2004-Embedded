#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
//const uint GP15_PIN = 15;

// Define the maximum length of strings to be injected or sniffed
const int MAX_STRING_LENGTH = 100;

// Declare default string to be sent. (To be changed to dynamic later on)
char *injectString = "Hello from Pico!\n";

// Function to send data based on GP15's state
void inject()
{
  // When inject mode is selected send the string 'Hello from Pico!' through UART0.
  uart_puts(UART_ID, injectString);
  sleep_ms(200); // Add a small delay after sending the string for consistency
}

void sniff()
{
  // When menu selected sniffing mode.
  char receivedString[MAX_STRING_LENGTH];
  int receivedStringIndex = 0;

  while (uart_is_readable(UART_ID))
  {
    char receivedChar = uart_getc(UART_ID);

    // Check if the received character is a newline character
    if (receivedChar == '\n')
    {
      // If a newline character is received, print the received string
      printf("Received string: %s\n", receivedString);

      // Added line to "forward" recieved message out after printing
      uart_puts(uart1, receivedString);

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
        // If the maximum length is reached, reset the received string index
        // and received string
        receivedStringIndex = 0;
        memset(receivedString, 0, sizeof(receivedString));
      }
    }
  }
}

// Function to test UART1 communication
// void test_uart1_tx()
// {
//   char *testString = "Hello from Pin 8!\n";
//   uart_puts(uart1, testString); // Change back to UART0 later
//   sleep_ms(200);
// }

// Function to test UART1 communication
// void test_uart1_rx()
// {
//   char receivedString[MAX_STRING_LENGTH];
//   int receivedStringIndex = 0;

//   while (uart_is_readable(uart1))
//   {
//     char receivedChar = uart_getc(uart1);

//     // Check if the received character is a newline character
//     if (receivedChar == '\n')
//     {
//       // If a newline character is received, print the received string
//       printf("Received from PIN9: %s\n", receivedString);

//       // Reset the received string index and received string
//       receivedStringIndex = 0;
//       memset(receivedString, 0, sizeof(receivedString));
//     }
//     else
//     {
//       // Append the received character to the received string
//       receivedString[receivedStringIndex++] = receivedChar;

//       // Check if the received string index reaches the maximum length
//       if (receivedStringIndex >= MAX_STRING_LENGTH)
//       {
//         // If the maximum length is reached, reset the received string index and
//         // received string
//         receivedStringIndex = 0;
//         memset(receivedString, 0, sizeof(receivedString));
//       }
//     }
//   }
//   sleep_ms(500);
// }

int main()
{
  // Initialize.
  stdio_init_all();

  // menu tracker things
  char menu_buffer[1024];
  int in_menu = 0;
  int menu_activated = 1;

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

  //gpio_init(GP15_PIN);

  // Set GP15 (GPIO15) as an input pin to read its state.
  //gpio_set_dir(GP15_PIN, GPIO_IN);
  //gpio_set_pulls(GP15_PIN, true, false);

  while (true)
  {
    if (scanf("%s", menu_buffer))
    {
      menu_activated = 0;
    }
    if (menu_activated == 0 && in_menu == 0)
    {
      print_menu();
      scanf("%s", menu_buffer);
      in_menu = 1;
      int uart_menu = 0;
      if (strcmp(menu_buffer, "2") == 0)
      {
        // selected UART

        printf("1. Sniff\n");
        printf("2. Inject\n");
        scanf("%d", &uart_menu);

        if (uart_menu == 1)
        {
          while (1)
          {
            //test_uart1_tx(); // Test function
            //sleep_ms(10);
            sniff();
            sleep_ms(500); // Wait for 0.5 seconds between cycles
          }
        }
        if (uart_menu == 2)
        {
          inject();
          //sleep_ms(10);
          //test_uart1_rx(); // Test function
          sleep_ms(500);   // Wait for 0.5 seconds between cycles
        }
      }

      if (strcmp(menu_buffer, "3") == 0)
      {
        int spi_menu = 0;
        //initialize spi buffers
        uint8_t message_buffer[1024];
        uint8_t spi0_input_buffer[1024];

        // not being used for now, need to test the below buffers
        // uint8_t spi1_input_buffer[1024];
        // uint8_t spi1_message_buffer[1024];

        //initialize spi interfaces and pins
        init_spi_pins(SPI_PORT, 500000, SPI0_RX_PIN, SPI0_TX_PIN, SPI0_SCLK_PIN, SPI0_CS_PIN);
        init_spi_pins(SPI1_PORT, 500000, SPI1_RX_PIN, SPI1_TX_PIN, SPI1_SCLK_PIN, SPI1_CS_PIN);
        spi_set_slave(SPI_PORT, true);

        printf("1. Sniff");
        printf("\n2. Inject\n");
        scanf("%d", &spi_menu);

        if (spi_menu == 1) {
          int in_spi_sniff = 0;
          // get number of bytes to sniff
          int number_of_bytes_input;
          printf("How many bytes of data do you want to sniff at a time?: \n");
          scanf("%d", number_of_bytes_input);
          while (in_spi_sniff != 1) {
            spi_write_read_blocking(SPI_PORT, spi0_input_buffer,message_buffer, number_of_bytes_input); // write to master on left, read from master on left,
            /*
            for (int i = 0; i < 4; i++) {
              relay_buffer[i] = message_buffer[i];
            }
            */
            spi_write_read_blocking(SPI1_PORT, message_buffer, spi0_input_buffer, number_of_bytes_input); // write to slave on right, read from slave on right

            //printing of output/logs
            for (int i = 0; i < 4; i++) {
              printf("Message from spi0: %x\n", message_buffer[i]);
              printf("Message write to spi1: %x\n", message_buffer[i]);
              printf("Message from spi1: %x\n", spi0_input_buffer[i]);
              printf("Message write to spi0: %x\n", spi0_input_buffer[i]);
            }
          }
        }
        if (spi_menu == 2)
        {
          uint8_t injected_input[1024];
          char user_hex_input[1024];
          // get user input as string
          printf("Enter bytes to send (eg. ff00112233): ");
          scanf("%s", user_hex_input);
          // convert the string to actual hex
          int input_len = count_char(user_hex_input);
          size_t hexa = convert_hex(injected_input, input_len, user_hex_input);
          int in_spi_inject = 0;
          while (in_spi_inject != 1) {
              // writing of injected bytes to slave, and relaying appropriate bytes back to master
              spi_write_read_blocking(SPI_PORT, spi0_input_buffer, message_buffer, hexa); //read from master on left, write to master on left
              spi_write_read_blocking(SPI1_PORT, injected_input, spi0_input_buffer, hexa); //write to slave with injected input on right, read from slave on right

              // printing of output/logs
              for (int i = 0; i < 4; i++) {
                printf("messaged from spi0: %x\n", message_buffer[i]);
                printf("messaged writing to spi1: %x\n", injected_input[i]);
                printf("received from spi1: %x\n", spi0_input_buffer[i]);
                printf("message write to spi0: %x\n", spi0_input_buffer[i]);
                printf("\n");
              }
            }
          }
        }
      }
    }

  return 0;
  }
