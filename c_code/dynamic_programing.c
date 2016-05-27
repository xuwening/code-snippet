
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//分冶法：将问题分解成独立的子问题
//【缺点：当子问题不独立，解就更复杂---所以使用下述的动态规划法】
int fibonacci(int a)
{
	if (a == 1 || a == 2)
		return 1;
	return fibonacci(a-1) + fibonacci(a-2);
}

//自底向上动态规划：
//通过计算按序排列的的所有函数值，从最小开始，在每一步使用前一步计算得到的值
//来计算当前值，这样可以计算任何这一类型的函数。
void dynamic_fibonacci_downup(int *buffer, int size)
{
	int i=2;
	buffer[0] = 1;
	buffer[1] = 1;
	for (i=2; i<size; i++)
	{
		buffer[i] = buffer[i-1] + buffer[i-2];
	}
}

//自顶向下动态规划：
//更简单的技术，与自底向上相比，它能够以一种自动化方式，花同样的开销来执行递归函数
//让这个递归程序保存它所计算的每个值【作为其后动作】，而且检查所保存的值，
//避免重新计算它们。
void dynamic_fibonacci_updown(int *buffer, int index)
{
	if (index == 0 || index == 1)
		return;

	dynamic_fibonacci_updown(buffer, index-1);
	//dynamic_fibonacci_updown(buffer, index-2);
	buffer[index] = buffer[index-1] + buffer[index-2];
}


//=============================================================
//测试
//=============================================================
void main(void)
{
	int i;

	int buffer[100];
	memset(buffer, 0, sizeof(buffer));

	//自底向上：动态规划法
	dynamic_fibonacci_downup(buffer, sizeof(buffer)/sizeof(buffer[0]));
	for (i=0; i<sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		printf("%d  ", buffer[i]);
	}

	printf("\n--------------------------\n");

	memset(buffer, 0, sizeof(buffer));
	buffer[0] = 0;
	buffer[1] = 1;

	//自顶向下：动态规划法
	dynamic_fibonacci_updown(buffer, 99);

	for (i=1; i<sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		printf("%d  ", buffer[i]);
	}

	//分冶法
/*
	for (i=1; i<100; i++)
	{
		printf("%d  ", fibonacci(i));
	}
*/
	getchar();
}

