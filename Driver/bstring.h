#ifndef STR_H
#define STR_H

#include "stm32f10x.h"
#include <stdio.h>

char *itoa(int value, char *string, int radix);
int atoi(const char *str);

#endif
