#ifndef UTIL_H
#define UTIL_H

#include "sllistc.h"
#include <stdio.h>

char *stolower(char *src);
void sl_list_print(struct sl_list_t *list, FILE *fs);

#endif