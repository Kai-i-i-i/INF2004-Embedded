#ifndef conversion_H_
#define conversion_H_

#include <stdint.h>
#include <stdio.h>

//function to convert the string hex input into uint8_t
size_t convert_hex(uint8_t *dest, size_t count, const char *src);

//function to count number of characters in a string
int count_char(char *user_string);

#endif //SPIFUNCS_H_