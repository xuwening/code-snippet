
//
//  1����������������3�룬ռ���ڴ治��13M
//

#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

#ifndef INT32
typedef int INT32;
#endif


#define BIT_MASK 0x1F
#define BIT_SHIFT_UNIT 5 

//λ�����ṹ������ͬʱʹ�ö��λ����
typedef struct tag_bit_vector {
	INT32 *bit_buffer;		//��������һ����λ����32��bit��
	INT32 max_bit_num;		//��bit��
}TBitVector;

//========================================================================================
//����һ��λ����
TBitVector *mallc_bit_vector(INT32 bit_max)
{
	TBitVector *pbit_vector = NULL;

	pbit_vector = (TBitVector *)malloc(sizeof(TBitVector));
	pbit_vector->bit_buffer = (INT32 *)calloc(sizeof(INT32), (bit_max + BIT_MASK) >> BIT_SHIFT_UNIT);
	pbit_vector->max_bit_num = bit_max;
	return pbit_vector;
}
//�ͷ�һ��λ����
void free_bit_vector(TBitVector *bit_vector)
{
	free(bit_vector->bit_buffer);
	free(bit_vector);
}

//========================================================================================

//����
void set_bit_vector(TBitVector *bit_vector, INT32 idx)
{
	bit_vector->bit_buffer[idx >> BIT_SHIFT_UNIT] |= 1 << (idx & BIT_MASK);
}
//���
void cls_bit_vector(TBitVector *bit_vector, INT32 idx)
{
	bit_vector->bit_buffer[idx >> BIT_SHIFT_UNIT] &= ~(1 << (idx & BIT_MASK));
}
//̽��
int test_bit(TBitVector *bit_vector, INT32 idx)
{
	return bit_vector->bit_buffer[idx >> BIT_SHIFT_UNIT] & (1 << (idx & BIT_MASK));
}

//========================================================================================
unsigned long sqrt_num = 0;

BOOL sievePrime(TBitVector *pBitVerctor, unsigned long prime)
{
	unsigned long i;

	assert(pBitVerctor != NULL);

	if (prime*prime >= pBitVerctor->max_bit_num)
	{
		return TRUE;
	}

	for (i=prime<<1; i<pBitVerctor->max_bit_num; i+=prime)
	{
		set_bit_vector(pBitVerctor, i);
	}

	return FALSE;
}

#define PRIME_NUMBER_MAX 100000000

//����
int main()
{
	int milisection = 0;
	int i = 0;
	int primeCount = 0;


	TBitVector *pdata = mallc_bit_vector(PRIME_NUMBER_MAX);
	
	milisection = GetTickCount();
	for (i=2; i<PRIME_NUMBER_MAX; i++)
	{
		if (test_bit(pdata, i))
			continue;

		if (sievePrime(pdata, i))
		{
			break;
		}
	}

	printf("times : %d\n", GetTickCount() - milisection);

	for (i=2; i<PRIME_NUMBER_MAX; i++)
	{
		if (test_bit(pdata, i) == 0)
		{
			primeCount++;
			//printf("%d    ", i);
		}
	}

	printf("prime count: %d\n", primeCount);

	free_bit_vector(pdata);

	getchar();
	return 0;
}