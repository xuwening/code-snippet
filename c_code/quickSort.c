
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

int partition(int *buffer, int leftIndex, int rightIndex)
{
	int key = buffer[leftIndex];

	while (leftIndex < rightIndex)
	{
		while (leftIndex < rightIndex && buffer[rightIndex] >= key)
			rightIndex--;
		buffer[leftIndex] = buffer[rightIndex];

		while (leftIndex < rightIndex && buffer[leftIndex] <= key)
			leftIndex++;
		buffer[rightIndex] = buffer[leftIndex];
	}

	buffer[leftIndex] = key;

	return leftIndex;
}

//¿ìËÙÅÅÐò
void quickSort(int *buffer, int leftIndex, int rightIndex)
{
	if (leftIndex < rightIndex)
	{
		int midIndex = partition(buffer, leftIndex, rightIndex);
		quickSort(buffer, leftIndex, midIndex-1);
		quickSort(buffer, midIndex+1, rightIndex);
	}
}


//²âÊÔ
int main()
{
	int i;
	int databuf[] = {10,  5, 6, 1, 0, 8, 11, 23, 14};

	quickSort(databuf, 0, sizeof(databuf)/sizeof(int)-1);

	for (i=0; i<sizeof(databuf)/sizeof(int); i++)
	{
		printf("%d  ", databuf[i]);
	}

	getchar();
	return 0;
}