// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "hardware/uart.h"
// #include "hardware/gpio.h"
// #include "pico/stdio/driver.h"

// #define UART_ID uart0
// #define BAUD_RATE 115200
// #define UART_TX_PIN 16
// #define UART_RX_PIN 17

// #define MAX_BUFFER_SIZE 128

// void receiveData()
// {
//     uint8_t buffer[MAX_BUFFER_SIZE];
//     while (uart_is_readable(UART_ID))
//     {
//         uart_read_blocking(UART_ID, buffer, 1);
//         printf("Received: %c\n", buffer[0]);
//         sleep_ms(500);
//     }
// }

// void sendData(char data)
// {
//     uart_write_blocking(UART_ID, (const uint8_t *)&data, 1);
//     printf("Sent successfully: %c\n", data);
//     sleep_ms(500);
// }

// int main()
// {
//     stdio_init_all();

//     // Configure UART pins
//     uart_init(UART_ID, BAUD_RATE);
//     gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//     gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

//     // Set UART pins
//     // uart_set_baudrate(UART_ID, BAUD_RATE);
//     // uart_set_hw_flow(UART_ID, false, false); // No hardware flow control

//     // Configure custom pins for input and output
//     gpio_init(UART_TX_PIN);
//     gpio_init(UART_RX_PIN);
//     gpio_set_dir(UART_TX_PIN, GPIO_OUT);
//     gpio_set_dir(UART_RX_PIN, GPIO_IN);

//     // while (true)
//     // {
//     //     // Mode Switching
//     //     char mode;
//     //     printf("Enter mode (1 for intercept, 2 for inject): ");
//     //     scanf(" %c", &mode); // Added a space before %c to consume the
//     newline character

//     //     if (mode == '1')
//     //     {
//     //         receiveData();
//     //     }
//     //     else if (mode == '2')
//     //     {
//     //         printf("Switched to inject mode. Enter data to send: ");
//     //         char data;
//     //         scanf(" %c", &data);
//     //         sendData(data);
//     //     }
//     // }

//     while (true)
//     {
//         // Check if there is data available to read
//         if (uart_is_readable(UART_ID))
//         {
//             char mode = uart_getc(UART_ID);

//             // Handle the newline character to avoid processing it as a mode
//             if (mode != '\n')
//             {
//                 if (mode == '1')
//                 {
//                     receiveData();
//                 }
//                 else if (mode == '2')
//                 {
//                     printf("Switched to inject mode. Enter data to send: ");
//                     char data = uart_getc(UART_ID);
//                     // scanf(" %c", &data);
//                     sendData(data);
//                 }
//             }
//         }
//     }

//     return 0;
// }

// #include <stdio.h>
// #include "mongoose.h"
// #include "pico/cyw43_arch.h"
// #include "pico/stdlib.h"

// #define WIFI_SSID ""
// #define WIFI_PASSWORD ""

// static void http_handler(struct mg_connection *c, int ev, void *ev_data,
//                          void *fn_data);

// int main() {
//   stdio_init_all();
//   if (cyw43_arch_init()) {
//     printf("failed to initialise\n");
//   }
//   cyw43_arch_enable_sta_mode();
//   printf("Connecting to Wi-Fi...\n");
//   if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
//                                          CYW43_AUTH_WPA2_AES_PSK, 30000)) {
//     printf("failed to connect.\n");
//     exit(1);
//   } else {
//     printf("Connected.\n");
//     // Print IP address after successful connection
//     printf("Connected to Wi-Fi. IP address: %s\n", cyw43_get_ip_address());
//   }

//   // Initialize Mongoose manager
//   struct mg_mgr mgr;
//   mg_mgr_init(&mgr);  // Init manager
//   mg_http_listen(&mgr, "http://0.0.0.0:8000", http_handler,
//                  NULL);              // Setup HTTP listener
//   for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop

//   return 0;
// }

// static void http_handler(struct mg_connection *c, int ev, void *ev_data,
//                          void *fn_data) {
//   if (ev == MG_EV_HTTP_MSG) {
//     struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//     if (mg_http_match_uri(hm, "/api/hello")) {
//       // On /api/hello requests, send dynamic JSON response
//       mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);
//     } else {
//       // For all other URIs, serve files from root_dir
//       struct mg_http_serve_opts opts = {.root_dir = "."};
//       mg_http_serve_dir(c, hm, &opts);
//     }
//   }
// }

#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

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

// Function to test UART1 communication
void test_uart1_tx() {
    char *testString = "Hello from Pin 8!\n";
    uart_puts(uart1, testString); // Change back to UART1 later
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

  while (true) {
    // test_uart1_tx(); // Call the function to test UART1 communication (TX)
    // test_uart1_rx(); // Call the function to test UART1 communication (RX)
    // send_data();     // Call the function to send or receive data based on
    // GP15's state sleep_ms(1000);   // Wait for 1 second between cycles

    if (gpio_get(GP15_PIN)) {
      inject();
      sleep_ms(10);
      test_uart1_rx();  // Call the function to test UART1 communication (RX) only when GP15 is high

    } else {
      test_uart1_tx();  // Call the function to test UART1 communication (TX) only when GP15 is low
      sleep_ms(10);
      sniff();
    }
    sleep_ms(500);  // Wait for 0.5 seconds between cycles
  }

  return 0;
}
