
#include "mstring.h"
#include <math.h>
#include <iostream>

#define Buffer_Length 50

unsigned int hash(const char* str) {
	unsigned int hv = 0, i = 0, p = 1;

	while (str[i] != '\0')
	{
		hv += p * int(str[i]);

		p *= 256;
		++i;
	}
	return hv;
}


void hash(unsigned int hv, char str[50]) 
{
	unsigned int i = 0, p = 256;

	while (p>1)
	{
		str[i] = char(hv % p);

		++i;
		p *= 256;
	}
	return ;
}


const char * hash(unsigned int hv)
{
	unsigned int i = 0, p = 256;
	char* str = new char[50]{0};
	
	while (p < hv)
	{
		str[i] = char(hv % p);

		++i;
		p *= 256;
	}
	return str;
}


int strcmpr(const char* str1, const char* str2)
{
	unsigned short i = 0;
	int hv = 0;

	do
	{
		hv = int(str1[i]) - int(str2[i]);
		//printf("%d", hv);
		i++;
	} while (
		(str1[i] != '\0') &&
		(str2[i] != '\0') &&
		hv == 0);

	return hv;

}