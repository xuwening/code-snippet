
#include "stdlib.h"
#include "string.h"

/*------------------------

//����˵һ�²��룺 �ò����ԭ���ǣ����Խ�����λ������λͳһ����
	���������㶼����ת��Ϊ�ӷ����㡣���λ�н�λ��������

�����Ĳ��룬��ԭ���������ͬ
�����Ĳ��룬������λ�⣬����λȡ����ĩβ��1


ģ�� ����ϵͳ�ĵļ�����Χ��10���Ƶ�ģ����10

//λ����
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
int test_bit(TBitVector *bit_verctor, INT32 idx)
{
	return bit_verctor->bit_buffer[idx >> BIT_SHIFT_UNIT] & (1 << (idx & BIT_MASK));
}

//========================================================================================

//����
int main()
{
	int i = 0;

	TBitVector *bitvec = mallc_bit_vector(100000);
	//����Ƿ��ʼ��
	for (i=0; i<100000; i++)
	{
		if (test_bit(bitvec, i))
		{
			printf("err..\n");
		}
	}

	//����ȫ��λ
	for (i=0; i<100000; i++)
	{
		set_bit_vector(bitvec, i);
	}

	//���ȫ��λ
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

	//������������ɶ�
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