
#include <stdlib.h>
#include <stdio.h>

//πÈ≤¢≈≈–Ú
void merge(int *array, int low, int mid, int height)
{
	int i,j, k;
	int *temp_buffer;
	int size = (height - low + 1) * sizeof(int);
	
	temp_buffer = malloc(size);
	if (temp_buffer == NULL)
		return;

	i=low;
	j = mid;
	k = 0;

	while (i < mid && j < height)
	{
		temp_buffer[k++] = array[i] < array[j] ? array[i++] : array[j++];
	}

	while (i < mid)
		temp_buffer[k++] = array[i++];

	while (j < height)
		temp_buffer[k++] = array[j++];

	memcpy(array, temp_buffer, (height-low) * sizeof(int));
	free(temp_buffer);
}

void merge_sort(int *array, int length)
{
	int mid = length / 2;
	if (mid >= 1)
	{
		merge_sort(array, mid);
		merge_sort(array+mid, length-mid);
		merge(array, 0, mid, length);
	}
}


int main(int argc, char **argv)
{
	int i;
	int buf[] = {5, 7, 3, 9, 2, 1, 8, 0, 6, 12, 33, 21, 18, 4, 9, 3, 7, 4, 8};
	merge_sort(buf, sizeof(buf)/sizeof(buf[0]));

	for (i=0; i<sizeof(buf)/sizeof(buf[0]); i++)
	{
		printf("%d ", buf[i]);
	}

	printf("\n");
	getchar();
	return 0;
}