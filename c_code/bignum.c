#include "stdio.h"
#include "windows.h"
#include "assert.h"
#include "math.h"

typedef struct tag_bigNum
{
	char *pNumber;  //�����洢�ռ�
	int totalCount; //�ռ��С
	int head;		//�������λ
	int tail;		//�������λ
	
	unsigned long fatorial;  //�׳���
}TBigNumber;

//����洢��������Ҫ�Ŀռ�
static int calcRequireBufferSize(unsigned long fatorial)
{
	//stirling��ʽ:  n! ~ sqrt(2*pi*n)*(n/e)^n
	return log10(fatorial) * fatorial;
}

//�����ڴ�ռ�
BOOL createBuffer(TBigNumber *pBigNumber, unsigned long fatorial)
{
	assert(pBigNumber != NULL);
	memset(pBigNumber, 0, sizeof(TBigNumber));
	
	pBigNumber->fatorial = fatorial;
	pBigNumber->totalCount = calcRequireBufferSize(fatorial);
	
	pBigNumber->pNumber = (char *)malloc(pBigNumber->totalCount);
	if (pBigNumber->pNumber == NULL)
		return FALSE;
	
	memset(pBigNumber->pNumber, 0, pBigNumber->totalCount);
	return TRUE;
}

//��һ�������
static void mulNextNumber(TBigNumber *pBigNumber, unsigned long num)
{
	unsigned long j;
	unsigned long temp=0;
	assert(pBigNumber != NULL && pBigNumber->pNumber != NULL);

	for (j=pBigNumber->tail; j>=pBigNumber->head; j--)
	{
		unsigned long prod = pBigNumber->pNumber[j] * num + temp;
		pBigNumber->pNumber[j] = (char)( prod % 10);
		temp = prod / 10;
	}
	
	while (temp > 0)
	{
		pBigNumber->head--;   
		pBigNumber->pNumber[pBigNumber->head] = (char)(temp % 10);
		temp /= 10;
	}
}

//����ĳ�����׳�
void calcFatorial(TBigNumber *pBigNumber)
{
	unsigned long i;
	
	assert(pBigNumber != NULL && pBigNumber->pNumber != NULL);
	
	pBigNumber->head = pBigNumber->totalCount-1;
	pBigNumber->tail = pBigNumber->head;
	pBigNumber->pNumber[pBigNumber->tail] = 1;
	
	for (i=2; i<=pBigNumber->fatorial; i++)
	{
		mulNextNumber(pBigNumber, i);
	}
}

//������
void printBigNumber(TBigNumber *pBigNumber)
{
	int i;

	FILE *hFile = fopen("result.txt", "wb+");
	if (hFile == NULL)
		return;
	
	assert(pBigNumber != NULL && pBigNumber->pNumber != NULL);
	for (i=pBigNumber->head; i<=pBigNumber->tail; i++)
		pBigNumber->pNumber[i] += '0';
		//printf("%d", pBigNumber->pNumber[i]);//+'0');

	fwrite(pBigNumber->pNumber + pBigNumber->head, 1
		, pBigNumber->totalCount-pBigNumber->head, hFile);

	printf("\n");
	printf("head: %d\n", pBigNumber->head);
	printf("tail: %d\n", pBigNumber->tail);
	printf("total: %d\n", pBigNumber->totalCount);
	printf("\n");

	fclose(hFile);
	hFile = NULL;
}

//�ͷ��ڴ�ռ�
void releaseBuffer(TBigNumber *pBigNumber)
{
	assert(pBigNumber != NULL);
	free(pBigNumber->pNumber);
	memset(pBigNumber, 0, sizeof(TBigNumber));
}

BOOL calcBigFactorialTest(unsigned long factorial)
{
	TBigNumber bigNum;
	if (!createBuffer(&bigNum, factorial))
		return FALSE;
	calcFatorial(&bigNum);
	printBigNumber(&bigNum);
	releaseBuffer(&bigNum);
}

void main(void)
{
	calcBigFactorialTest(100000);
	getchar();
}