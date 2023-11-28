#ifndef conversion_H_
#define conversion_H_

#include <stdint.h>
#include <stdio.h>

size_t convert_hex(uint8_t *dest, size_t count, const char *src);

int count_char(char *user_string);

#endif //SPIFUNCS_H_