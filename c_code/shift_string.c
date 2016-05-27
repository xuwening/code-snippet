#include "stdlib.h"
#include "string.h"
#include "stdio.h"


//·­×ª×Ö·û´®
bool convert_data(char *pdata, unsigned int start, unsigned int end)
{
	char ch;
	if (pdata == NULL || start > end)
		return false;

	while (start < end)
	{
		ch = pdata[start];
		pdata[start] = pdata[end];
		pdata[end] = ch;

		start++;
		end--;
	}

	return true;
}

//×Ö·û´®Ñ­»·ÒÆ¶¯
char *shift_data(char *pdata, unsigned int datalen, unsigned int shiftnum)
{
	if (pdata == NULL || shiftnum > datalen-1)
		return NULL;

	convert_data(pdata, 0, shiftnum-1);
	convert_data(pdata, shiftnum, datalen-1);
	convert_data(pdata, 0, datalen-1);

	return pdata;
}

//²âÊÔ
int main()
{
	char teststring[] = "abcdefghigklmn";
	printf(teststring);
	printf("\n");
	shift_data(teststring, strlen(teststring), 4);

	printf(teststring);

	getchar();
	return 0;
}