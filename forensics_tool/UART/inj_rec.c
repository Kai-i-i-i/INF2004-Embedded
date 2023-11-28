#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "menu/menu.h"
#include "SPIFUNCS/SPIFUNCS.h"
#include "conversion/conversion.h"

#define UART_COMMS_ID uart1
#define BAUD_RATE_COMMS 115200

#define UART_CONSOLE_ID uart0
#define BAUD_RATE_CONSOLE 9600

// Setting the pins for TX and RX.
#define UART_TX_PIN 16
#define UART_RX_PIN 17

#define UART_TX_PIN_TESTING 8
#define UART_RX_PIN_TESTING 9

// Redirect stdout to UART_CONSOLE_ID
FILE __stdout;
#define stdout (&__stdout)


// Setting pin 15 as a pseudo button.
// const uint GP15_PIN = 15;

// Declare default string to be sent. (To be changed to dynamic later on)
char *injectString = "Hello from Pico!\n";

// Function to send data based on GP15's state
void inject()
{
  // When inject mode is selected send the string 'Hello from Pico!' through UART0.
  uart_puts(UART_COMMS_ID, injectString);
  sleep_ms(200); // Add a small delay after sending the string for consistency
}

void sniff()
{
  // When menu selected sniffing mode.
  printf("In Sniff function\n");
  char receivedString[1024];
  int receivedStringIndex = 0;

  if (uart_is_readable(UART_COMMS_ID))
  {
    printf("Data available for reading\n");
    while (uart_is_readable(UART_COMMS_ID))
    {
      printf("First line in while loop\n");
      char receivedChar = uart_getc(UART_COMMS_ID);
      printf("%c\n", receivedChar);

      // Check if the received character is a newline character
      if (receivedChar == '\n')
      {
        // If a newline character is received, print the received string
        printf("Received string: %s\n", receivedString);

        // Added line to "forward" recieved message out after printing
        // uart_puts(uart1, receivedString);

        // Reset the received string index and received string
        receivedStringIndex = 0;
        memset(receivedString, 0, sizeof(receivedString));
      }
      else
      {
        // Append the received character to the received string
        receivedString[receivedStringIndex++] = receivedChar;

        // Check if the received string index reaches the maximum length
        if (receivedStringIndex >= sizeof(receivedString))
        {
          // If the maximum length is reached, reset the received string index
          // and received string
          receivedStringIndex = 0;
          memset(receivedString, 0, sizeof(receivedString));
        }
      }
    }
  }
}

// Function to test UART1 communication
void test_tx()
{
  char *testString = "Hello from Pin 8!\n";
  uart_puts(UART_COMMS_ID, testString);
  sleep_ms(200);
}

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

  // Initialize UART0 for serial console
  uart_init(UART_CONSOLE_ID, BAUD_RATE_CONSOLE);

  // Initialize UART1
  uart_init(UART_COMMS_ID, BAUD_RATE_COMMS);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  printf("UART 1 Initialized\n");

  // menu tracker things
  char menu_buffer[1024];
  int in_menu = 0;
  int menu_activated = 1;

  // gpio_set_function(UART_TX_PIN_TESTING, GPIO_FUNC_UART);
  // gpio_set_function(UART_RX_PIN_TESTING, GPIO_FUNC_UART);

  while (true)
  {
    // Non-blocking console input
    if (uart_is_readable(UART_CONSOLE_ID))
    {
      if (scanf("%s", menu_buffer))
      {
        menu_activated = 0;
      }
    }

    if (menu_activated == 0 && in_menu == 0)
    {
      print_menu();
      scanf("%s", menu_buffer);
      in_menu = 1;

      if (strcmp(menu_buffer, "2") == 0)
      {
        // selected UART

        printf("1. Sniff\n");
        printf("2. Inject\n");

        int uart_menu = 0;
        scanf("%d", &uart_menu);

        if (uart_menu == 1)
        {
          // while (1)

          test_tx(); // Test function
          printf("testString sent\n");
          sleep_ms(10);
          sniff();
          sleep_ms(500); // Wait for 0.5 seconds between cycles
        }
        if (uart_menu == 2)
        {
          inject();
          // sleep_ms(10);
          // test_uart1_rx(); // Test function
          sleep_ms(500); // Wait for 0.5 seconds between cycles
        }
      }

      if (strcmp(menu_buffer, "3") == 0)
      {
        int spi_menu = 0;
        //initialize spi buffers
        //uint8_t message_buffer[1024];
        //uint8_t spi0_input_buffer[1024];

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

          //create buffers for spi
          uint8_t *write_buffer = create_buffers(number_of_bytes_input);
          uint8_t *read_buffer = create_buffers(number_of_bytes_input);
          while (in_spi_sniff != 1) {
            //spi_write_read_blocking(SPI_PORT, spi0_input_buffer,message_buffer, number_of_bytes_input); // write to master on left, read from master on left,
            /*
            for (int i = 0; i < 4; i++) {
              relay_buffer[i] = message_buffer[i];
            }
            */
            //spi_write_read_blocking(SPI1_PORT, message_buffer, spi0_input_buffer, number_of_bytes_input); // write to slave on right, read from slave on right
            sniff_spi(write_buffer, read_buffer, number_of_bytes_input);
            //printing of output/logs
            for (int i = 0; i < number_of_bytes_input; i++) {
              printf("Message from spi0: %x\n", read_buffer[i]);
              printf("Message write to spi1: %x\n", read_buffer[i]);
              printf("Message from spi1: %x\n", write_buffer[i]);
              printf("Message write to spi0: %x\n", write_buffer[i]);
            }
          }
        }
        if (spi_menu == 2)
        {

          char user_hex_input[1024];

          // get user input as string
          printf("Enter bytes to send (eg. ff00112233): ");
          scanf("%s", user_hex_input);

          // convert the string to actual hex
          int input_len = count_char(user_hex_input);
          uint8_t *injected_input = create_buffers(input_len);
          size_t hexa = convert_hex(injected_input, input_len, user_hex_input);

          //create the buffers for spi
          uint8_t *write_buffer = create_buffers(hexa);
          uint8_t *read_buffer = create_buffers(hexa);

          int in_spi_inject = 0;
          while (in_spi_inject != 1) {
              // writing of injected bytes to slave, and relaying appropriate bytes back to master
              //spi_write_read_blocking(SPI_PORT, write_buffer, read_buffer, hexa); //read from master on left, write to master on left
              //spi_write_read_blocking(SPI1_PORT, injected_input, write_buffer, hexa); //write to slave with injected input on right, read from slave on right

              inject_spi(write_buffer, read_buffer, injected_input, hexa);
              // printing of output/logs
              for (int i = 0; i < hexa; i++) {
                printf("messaged from spi0: %x\n", read_buffer[i]);
                printf("messaged writing to spi1: %x\n", injected_input[i]);
                printf("received from spi1: %x\n", write_buffer[i]);
                printf("message write to spi0: %x\n", write_buffer[i]);
                printf("\n");
              }
            }
          }
        }
      }
    }
    // Reset menu_activated after processing the menu
    menu_activated = 1;
    return 0;
  }

