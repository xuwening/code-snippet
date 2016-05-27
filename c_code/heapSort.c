#include "stdio.h"
#include "stdlib.h"

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void adjustHeap(int *array, int len, int idx)
{
	int left = idx*2+1;
	int right = idx*2+2;
	int max_idx = idx;

	if (left <=len)
		max_idx = array[max_idx] >= array[left] ? max_idx : left;
	if (right <= len)
		max_idx = array[max_idx] >= array[right] ? max_idx : right;
	if (max_idx != idx)
	{
		swap(&array[max_idx], &array[idx]);
		adjustHeap(array, len, max_idx);
	}
}

void createHeap(int *array, int len)
{
	int i;
	for (i=len/2-1; i>=0; i--)
	{
		adjustHeap(array, len-1, i);
	}
}

//╤яее
void heapSort(int *array, int len)
{
	int i;
	createHeap(array, len);
	for (i=len-1; i>=0; i--)
	{
		swap(&array[0], &array[i]);
		adjustHeap(array, i-1, 0);
	}
}



void main(void)
{
	int arr[] = {0, 9, 2, 4, 8, 5, 7, 3};
	heapSort(arr, sizeof(arr)/sizeof(arr[0]));
	
	{
		int i;
		for (i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			printf("%d  ", arr[i]);
		}
	}

	getchar();
}

