#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

size_t convert_hex(uint8_t *dest, size_t count, const char *src) {
    char buf[3];
    size_t i;
    for (i = 0; i < count && *src; i++) {
        buf[0] = *src++;
        buf[1] = '\0';
        if (*src) {
            buf[1] = *src++;
            buf[2] = '\0';
        }
        if (sscanf(buf, "%hhx", &dest[i]) != 1)
            break;
    }
    return i;
}

int count_char(char *user_string) {
    int i = 0;
    int total_char = 0;
    while (user_string[i] != '\0') {
        total_char++;
        i++;
    }
    return total_char;
}

