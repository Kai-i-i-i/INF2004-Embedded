#include "menu.h"
#include <stdio.h>

void myfunc() {
    printf("hello\n");
}

void print_menu() {
    printf("Welcome to BoozePiret\n");
    printf("1. View info\n");
    printf("2. UART Sniff\n");
    printf("3. SPI Sniff\n");
    printf("> ");
}