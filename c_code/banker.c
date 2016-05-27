
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>


/*************************
演示程序：非正式算法
*************************/

//银行家算法:银行家在客户申请的贷款数量不超过自己拥有的最大值时，都应尽量满足客户的需要。

//每个客户以用资源表
//每个客户最多需要资源表
//银行还剩余资源表
//客户完成度

#define SOURCE_TYPE_NUM 4
#define CUSTOMER_NUM 4

typedef struct tag_banker 
{
	int source[SOURCE_TYPE_NUM];

	int customer[CUSTOMER_NUM][SOURCE_TYPE_NUM];
	int custMax[CUSTOMER_NUM][SOURCE_TYPE_NUM];

	int currentNeed[SOURCE_TYPE_NUM];
	bool complete[CUSTOMER_NUM];
}TBanKer;

TBanKer banker = {
	{10, 10, 10, 10},
	{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{5, 7, 2, 3}, {4, 5, 7, 7}, {0, 2, 5, 5}, {8, 8, 9, 4}},
	{0, 0, 0, 0},
	{false, false, false, false},
};


void displaySource(TBanKer *pBanker)
{
	printf("剩余资源：%d  %d  %d  %d\n", pBanker->source[0], pBanker->source[1], pBanker->source[2], pBanker->source[3]);
}

void retrieveSource(TBanKer *pBanker, int index)
{
	int i;
	for (i=0; i<SOURCE_TYPE_NUM; i++)
	{
		pBanker->source[i] += pBanker->customer[index][i];
	}
}

void calcComplete(TBanKer *pBanker, int index)
{
	int i;
	int result = 0;
	if (pBanker->complete[index])
		return;

	for (i=0; i<SOURCE_TYPE_NUM; i++)
	{
		if (pBanker->customer[index][i] == pBanker->custMax[index][i])
			result++;
	}

	if (result == SOURCE_TYPE_NUM)
	{
		pBanker->complete[index] = true;
		retrieveSource(pBanker, index);
		printf("客户%d业务完成！\n", index);
		displaySource(pBanker);
	}
}

bool calcNeedSource(TBanKer *pBanker, int index)
{
	int i;
	bool ret = false;
	int result[SOURCE_TYPE_NUM];

	for (i=0; i<SOURCE_TYPE_NUM; i++)
	{
		pBanker->currentNeed[i] = pBanker->custMax[index][i] - pBanker->customer[index][i];
		result[i] = pBanker->source[i] - pBanker->currentNeed[i];
		if (result[i] < 0)
		{
			printf("客户%d资源不满足，等待下次分配。\n", index);
			break;
		}
	}

	if (i == SOURCE_TYPE_NUM)
		ret = true;
	return ret;
}

void allocSource(TBanKer *pBanker, int index)
{
	int i;

	printf("分配给客户%d资源：", index);
	for (i=0; i<SOURCE_TYPE_NUM; i++)
	{
		pBanker->source[i] -= pBanker->currentNeed[i];
		pBanker->customer[index][i] += pBanker->currentNeed[i];
		printf("%d ", pBanker->currentNeed[i]);
	}
	printf("\n");
	displaySource(pBanker);
}


void allocationSource(TBanKer *pBanker)
{
	int i;

	displaySource(pBanker);

	while (true)
	{

		for (i=0; i<CUSTOMER_NUM; i++)
		{
			if (pBanker->complete[i])
				continue;
			
			if (calcNeedSource(pBanker, i))
				allocSource(pBanker, i);
		}

		for (i=0; i<CUSTOMER_NUM; i++)
		{
			calcComplete(pBanker, i);
		}
		
		if (pBanker->complete[0] & pBanker->complete[1] & pBanker->complete[2] & pBanker->complete[3])
			break;

		Sleep(100);
	}
}

//测试
int main()
{
	int a = 0x12345678;
	int b = 0x12345678;


	allocationSource(&banker);

	getchar();
	return 0;
}