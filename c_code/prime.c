
//
//  1亿以下素数，不到3秒，占用内存不到13M
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

//位向量结构：方便同时使用多个位向量
typedef struct tag_bit_vector {
	INT32 *bit_buffer;		//数据区【一个单位包含32个bit】
	INT32 max_bit_num;		//总bit数
}TBitVector;

//========================================================================================
//分配一个位向量
TBitVector *mallc_bit_vector(INT32 bit_max)
{
	TBitVector *pbit_vector = NULL;

	pbit_vector = (TBitVector *)malloc(sizeof(TBitVector));
	pbit_vector->bit_buffer = (INT32 *)calloc(sizeof(INT32), (bit_max + BIT_MASK) >> BIT_SHIFT_UNIT);
	pbit_vector->max_bit_num = bit_max;
	return pbit_vector;
}
//释放一个位向量
void free_bit_vector(TBitVector *bit_vector)
{
	free(bit_vector->bit_buffer);
	free(bit_vector);
}

//========================================================================================

//设置
void set_bit_vector(TBitVector *bit_vector, INT32 idx)
{
	bit_vector->bit_buffer[idx >> BIT_SHIFT_UNIT] |= 1 << (idx & BIT_MASK);
}
//清除
void cls_bit_vector(TBitVector *bit_vector, INT32 idx)
{
	bit_vector->bit_buffer[idx >> BIT_SHIFT_UNIT] &= ~(1 << (idx & BIT_MASK));
}
//探测
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

//测试
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