
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>


//—°‘Ò≈≈–Ú
void selectSort(int *buffer, int bufferSize)
{
	int i,k;
	int min, temp;

	for (i=0; i<bufferSize; i++)
	{
		min = i;

		for (k=i; k<bufferSize; k++)
		{
			if (buffer[min] > buffer[k])
				min = k;
		}

		temp = buffer[i];
		buffer[i] = buffer[min];
		buffer[min] = temp;
	}
}

//≤‚ ‘
int main()
{
	int i;
	int databuf[] = {10,  5, 6, 1, 0, 8, 11, 23, 14};

	selectSort(databuf, sizeof(databuf)/sizeof(int));

	for (i=0; i<sizeof(databuf)/sizeof(int); i++)
	{
		printf("%d  ", databuf[i]);
	}

	getchar();
	return 0;
}