
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>


//√∞≈›≈≈–Ú
void bubbleSort(int *pBuffer, int bufLen)
{
	int i,k;
	BOOL bExchange;
	int temp;

	for (i=0; i<bufLen; i++)
	{
		bExchange = FALSE;
		for (k=bufLen-1; k>i; k--)
		{
			if (pBuffer[k] < pBuffer[k-1])
			{
				bExchange = TRUE;

				temp = pBuffer[k];
				pBuffer[k] = pBuffer[k-1];
				pBuffer[k-1] = temp;
			}
		}

		if (!bExchange)
			break;
	}
}



//≤‚ ‘
int main()
{
	int i;
	int databuf[] = {3,  5, 12, 1, 23, 0, 8, 1, 14};

	bubbleSort(databuf, sizeof(databuf)/sizeof(int));

	for (i=0; i<sizeof(databuf)/sizeof(int); i++)
	{
		printf("%d  ", databuf[i]);
	}

	getchar();
	return 0;
}