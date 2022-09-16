#pragma once

#include <string.h>

#define Buffer_Length 50

unsigned int hash(const char* str);

void hash(unsigned int hv, char str[50]);

const char* hash(unsigned int hv);

int strcmpr(const char* str1, const char* str2);
