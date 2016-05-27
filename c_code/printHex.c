
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>


char my_hex(char ch)
{
	char temp = (ch & 0x0F);
	return (temp > 9 ? (temp-10) + 'A' : temp + '0');
}

void printfStream(char *addr, unsigned int len, unsigned int colNum, unsigned int groupNum)
{
	int i;

	if (addr == NULL)
		return;

	for (i=0; i<len; i++)
	{
		if ((i%colNum) == 0)
		{
			printf("\n");
			printf("0x%X: ", addr+i);
		} else if ((i%groupNum) == 0)
		{
			printf("  ");
		}

		printf("0x%c%c ", my_hex(addr[i] >> 4), my_hex(addr[i]));
		//printf("0x%X ", addr[i]);
	}

	printf("\n");
}

//¥Ú”°¬Î¡˜
#define PRINT_STREAM(ADDR, LEN) printfStream(ADDR, LEN, 12, 4);


//≤‚ ‘
int main()
{
	int a = 0x12345678;
	int b = 0x12345678;

	PRINT_STREAM((char *)&a, 44);

	getchar();
	return 0;
}