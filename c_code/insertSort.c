
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

//≤Â»Î≈≈–Ú
void insertSort(int *buffer, int bufferSize)
{
	int i,j;
	int key;

	for (i=1; i<bufferSize;i++)
	{
		key = buffer[i];

		for (j=i; j > 0 && key < buffer[j-1]; j--)
		{
			buffer[j] = buffer[j-1];
		}

		buffer[j] = key;
	}
}

//≤‚ ‘
int main()
{
	int i;
	int databuf[] = {10,  5, 6, 1, 0, 8, 11, 23, 14};

	insertSort(databuf, sizeof(databuf)/sizeof(int));

	for (i=0; i<sizeof(databuf)/sizeof(int); i++)
	{
		printf("%d  ", databuf[i]);
	}

	getchar();
	return 0;
}