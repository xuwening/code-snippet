
#include "stdlib.h"
#include "string.h"

/*------------------------

//首先说一下补码： 用补码的原因是，可以将符号位与其他位统一处理
	，所有运算都可以转换为加法运算。最高位有进位可以舍弃

正数的补码，与原码二进制相同
负数的补码，除符号位外，其他位取反，末尾加1


模： 计量系统的的计数范围，10进制的模就是10

//位操作
int a = -1;
int c = a >> 1;  // c == -1
int c = a / 2;   // c == 0

int c = a % 32;  // == a & (32 - 1)


------------------------*/

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
int test_bit(TBitVector *bit_verctor, INT32 idx)
{
	return bit_verctor->bit_buffer[idx >> BIT_SHIFT_UNIT] & (1 << (idx & BIT_MASK));
}

//========================================================================================

//测试
int main()
{
	int i = 0;

	TBitVector *bitvec = mallc_bit_vector(100000);
	//检测是否初始化
	for (i=0; i<100000; i++)
	{
		if (test_bit(bitvec, i))
		{
			printf("err..\n");
		}
	}

	//设置全部位
	for (i=0; i<100000; i++)
	{
		set_bit_vector(bitvec, i);
	}

	//检测全部位
	for (i=0; i<100000; i++)
	{
		if (test_bit(bitvec, i))
		{
			cls_bit_vector(bitvec, i);
			continue;
		} else {
			break;
		}
	}

	if (i != 100000)
	{
		printf("err..\n");
	}

	//检测清除操作完成度
	for (i=0; i<100000; i++)
	{
		if (test_bit(bitvec, i))
		{
			printf("err..\n");
		}
	}

	if (i != 100000)
	{
		printf("err..\n");
	}
	else
	{
		printf("ok...\n");
	}


	free(bitvec);
	bitvec = NULL;

	getchar();
	return 0;
}