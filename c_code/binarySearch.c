//¶ş·ÖËÑË÷·¨
int binarySearch(int *buffer, int left, int right, int data)
{
	int mid;

	while (left <= right)
	{
		mid = left + (right-left) / 2;

		if (buffer[mid] == data)
			return mid;
		else if (buffer[mid] > data)
			right = mid - 1;
		else
			left = mid + 1;
	}

	return -1;
}

