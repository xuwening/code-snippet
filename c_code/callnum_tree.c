#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>

#include "vld.h"

#define CALL_NUM_LENGHT 11

typedef char NodeType;

typedef struct tag_treeNode 
{
	struct tag_treeNode *child;
	struct tag_treeNode *brother;
	NodeType val;
}TreeNode;

typedef struct tag_Tree 
{
	TreeNode *root;
	//unsigned long nodeCount;
}Tree;

//创建树
Tree *createTree()
{
	Tree *pTree = (Tree *)malloc(sizeof(Tree));
	if (pTree)
		memset(pTree, 0, sizeof(Tree));

	return pTree;
}

//释放树
void releaseTree(Tree *tree)
{
	free(tree);
}

//添加根节点
TreeNode *addRootNode(Tree *pTree, NodeType val)
{
	TreeNode *tempNode = (TreeNode *)malloc(sizeof(TreeNode));
	if (tempNode == NULL)
		return NULL;
	tempNode->val = val;
	tempNode->brother = NULL;
	tempNode->child = NULL;
	pTree->root = tempNode;
	return tempNode;
}

//添加子节点
TreeNode *addChildNode(TreeNode *prentNode, NodeType val)
{
	TreeNode *tempNode = (TreeNode *)malloc(sizeof(TreeNode));
	if (tempNode == NULL)
		return NULL;

	tempNode->brother = prentNode->child;
	tempNode->val = val;
	tempNode->child = NULL;
	prentNode->child = tempNode;
	return tempNode;
}

//删除某节点
TreeNode *delChildNode(TreeNode *pCurrentNode)
{
	TreeNode *pNextNode;
	if (pCurrentNode == NULL)
		return NULL;
	pNextNode = pCurrentNode->child;

	free(pCurrentNode);
	return pNextNode;
}

//添加兄弟节点
TreeNode *addBortherNode(TreeNode *bortherNode, NodeType val)
{
	TreeNode *tempNode;
	if (bortherNode == NULL)
		return FALSE;
	tempNode = (TreeNode *)malloc(sizeof(TreeNode));
	if (tempNode == NULL)
		return NULL;
	
	tempNode->brother = bortherNode->brother;
	bortherNode->brother = tempNode;
	tempNode->val = val;
	tempNode->child = NULL;
	return tempNode;
}

//查找节点
TreeNode *findChildNode(TreeNode *prentNode, NodeType val)
{
	TreeNode *ptempNode;
	if (prentNode == NULL)
		return NULL;
	if (prentNode->child == NULL)
		return NULL;
	
	ptempNode = prentNode->child;
	while (ptempNode)
	{
		if (ptempNode->val == val)
			return ptempNode;
		ptempNode = ptempNode->brother;
	}
	
	return NULL;
}

//查找相近节点
TreeNode *findNearBrotherNode(TreeNode *prentNode, NodeType val)
{
	TreeNode *pPreNode;
	TreeNode *ptempNode;
	if (prentNode == NULL)
		return NULL;
	if (prentNode->child == NULL)
		return NULL;

	pPreNode = prentNode;
	ptempNode = prentNode->child;
	while (ptempNode)
	{
		if (ptempNode->val < val)
		{
			pPreNode = ptempNode;
			ptempNode = ptempNode->brother;
		}
		else if (ptempNode->val == val)
		{
			pPreNode = ptempNode;
			break;
		}
		else
		{
			break;
		}
	}

	return pPreNode;
}

//查找前驱节点
TreeNode *findPreNode(TreeNode *prentNode, TreeNode *desNode)
{
	TreeNode *pPreNode;

	if (prentNode == NULL || desNode == NULL)
		return NULL;
	if (prentNode->child == NULL)
		return NULL;
	
	pPreNode = prentNode->child;
	if (pPreNode == desNode)
		return prentNode;

	while (prentNode)
	{
		if (pPreNode->brother == desNode)
			return pPreNode;
		pPreNode = pPreNode->brother;
	}
	
	return NULL;
}


//查找某号码
char *searchCallNumber(Tree *pTree, NodeType *callNum, int len)
{
	int i;
	TreeNode *ptempNode;

	if (pTree == NULL)
		return NULL;
	if (pTree->root == NULL)
		return NULL;

	ptempNode = pTree->root;
	if (ptempNode->val != callNum[0])
		return NULL;

	for (i=1; i<len; i++)
	{
		ptempNode = findChildNode(ptempNode, callNum[i]);
		if (ptempNode == NULL)
			return NULL;
	}

	printf("success.\n");
	return (char *)ptempNode->child; //最后一个节点child指针指向用户名，或数据结构
}

//增加某号码
BOOL addCallNumber(Tree *pTree, NodeType *callNum, int len)
{
	int i;
	TreeNode *ptempNode = pTree->root;
	if (callNum == NULL || pTree == NULL)
		return FALSE;

	if (ptempNode == NULL)
	{
		ptempNode = addRootNode(pTree, callNum[0]);//增加头节点
		if (ptempNode == NULL)
			return FALSE;
	}

	for (i=1; i<len; i++)
	{
		if (ptempNode == NULL)
			return FALSE;

		if (ptempNode->child == NULL || ptempNode->child->val > callNum[i])
		{
			ptempNode = addChildNode(ptempNode, callNum[i]);
			continue;
		}

		//找出增加节点的位置
		ptempNode = findNearBrotherNode(ptempNode, callNum[i]);
		if (ptempNode != NULL)
		{
			if (ptempNode->val != callNum[i])
				ptempNode = addBortherNode(ptempNode, callNum[i]);
		}
	}

	return TRUE;
}

int getDelIndex(BOOL *hasBrother, int length)
{
	if (hasBrother == NULL)
		return -2;

	while (length--)
	{
		if (hasBrother[length] == 1)
			return (length-1);
	}

	return -1;
}

//删除号码
BOOL delCallNumber(Tree *pTree, NodeType *callNum, int len)
{
	int i;
	int delIndex = 0;
	TreeNode *ptempNode;

	TreeNode *delList[CALL_NUM_LENGHT];
	BOOL hasBorther[CALL_NUM_LENGHT];
	memset(delList, 0, sizeof(delList));
	memset(hasBorther, 0, sizeof(hasBorther));

	if (pTree == NULL || callNum == NULL)
		return FALSE;

	delList[0] = pTree->root;
	if (delList[0] == NULL)
		return FALSE;
	if (delList[0]->val != callNum[0])
		return FALSE;

	for (i=1; i<CALL_NUM_LENGHT; i++)
	{
		ptempNode = findChildNode(delList[i-1], callNum[i]);
		if (ptempNode == NULL)
		{
			printf("\ncall number not exist.\n");
			return FALSE;
		}

		hasBorther[i] = delList[i-1]->child->brother != NULL ? TRUE : FALSE;
		delList[i] = ptempNode;
	}

	delIndex = getDelIndex(hasBorther, sizeof(hasBorther)/sizeof(hasBorther[0]));
	if (delIndex != -2)
	{	
		if (delIndex == -1)
		{
			pTree->root = NULL;
			ptempNode = delList[0];
		}
		else
		{
			//有兄弟节点
			ptempNode = findPreNode(delList[delIndex], delList[delIndex+1]);
			if (ptempNode == NULL)
				return FALSE;

			if (ptempNode == delList[delIndex]) //左
				delList[delIndex]->child = delList[delIndex+1]->brother;
			else
				ptempNode->brother = delList[delIndex+1]->brother;

			ptempNode = delList[delIndex+1];
		}

		while (ptempNode)
		{
			ptempNode = delChildNode(ptempNode);
		}
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}

void printfCallNumber(NodeType *callnum, int len)
{
	while (len--)
	{
		printf("%d", *callnum++);
	}
	printf("\n");
}

void displayAllCallNumber(TreeNode *pNode, NodeType *callnum, int lenght)
{
	if (pNode == NULL || callnum == NULL)
		return ;

	callnum[lenght] = pNode->val;

	if (lenght == CALL_NUM_LENGHT-1)
	{
		printfCallNumber(callnum, lenght+1);
		displayAllCallNumber(pNode->brother, callnum, lenght);
		return ;
	} 


	displayAllCallNumber(pNode->child, callnum, lenght+1);
	displayAllCallNumber(pNode->brother, callnum, lenght);
}

void displayTree(Tree *pTree)
{
	if (pTree != NULL)
	{
		NodeType desNum[12] = {0};
		displayAllCallNumber(pTree->root, desNum, 0);
	}
}


//测试
//=========================================================
void incCallNum(NodeType *callnum, int len)
{
	int i;

	callnum[len-1] += 1;
	for (i=len-1; i>0; i--)
	{
		if (callnum[i] > 9)
		{
			callnum[i] -= 10;
			callnum[i-1] += 1;
		}
		else
		{
			break;
		}
	}
}

void decCallNum(NodeType *callnum, int len)
{
	int i;
	
	callnum[len-1] -= 1;
	for (i=len-1; i>0; i--)
	{
		if (callnum[i] < 0)
		{
			callnum[i] += 10;
			callnum[i-1] -= 1;
		}
		else
		{
			break;
		}
	}
}

#define TIMES_COUNT 1000000

int main()
{
	int i;
	int times;
	//NodeType dynamicNum[] = {1, 3, 0, 3, 5, 3, 8, 1, 6, 9, 4};
// 	NodeType callNum[][CALL_NUM_LENGHT] = 
// 	{
// 		{1, 3, 3, 3, 5, 3, 8, 1, 6, 9, 4},
// 		{1, 3, 3, 3, 5, 3, 8, 1, 6, 9, 7},
// 		{1, 3, 3, 3, 5, 3, 8, 1, 6, 8, 4},
// 		{1, 3, 3, 3, 5, 3, 8, 1, 6, 8, 5},
// 		{1, 3, 3, 3, 5, 3, 8, 1, 5, 3, 5},
// 		{1, 3, 3, 3, 5, 3, 8, 1, 5, 2, 5},
// 		
// 		{1, 2, 3, 3, 5, 3, 8, 1, 6, 9, 4},
// 		{1, 2, 4, 3, 5, 3, 8, 1, 6, 9, 7},
// 		{1, 5, 3, 3, 5, 3, 8, 1, 6, 8, 4},
// 		{1, 4, 3, 3, 5, 3, 8, 1, 6, 8, 5},
// 		{1, 8, 3, 3, 5, 3, 8, 1, 5, 3, 5},
// 		{1, 9, 3, 3, 5, 3, 8, 1, 5, 2, 5},
// 	};

	NodeType dynamicNum[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	NodeType dynamicNumC[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	//times = GetTickCount();
	
	Tree *pTree;
	pTree = createTree();
	for (i=0; i<TIMES_COUNT; i++)
	{
		BOOL ret;
		incCallNum(dynamicNum, sizeof(dynamicNum));
		//printfCallNumber(dynamicNum, sizeof(dynamicNum));
		ret = addCallNumber(pTree, dynamicNum, sizeof(dynamicNum));
		if (ret == FALSE)
			printf("add node failed.\n");
	}
	
	printf("del...\n");
	//getchar();
	for (i=0; i<TIMES_COUNT; i++)
	{
		delCallNumber(pTree, dynamicNum, sizeof(dynamicNum));
		//printfCallNumber(dynamicNum, sizeof(dynamicNum));
		decCallNum(dynamicNum, sizeof(dynamicNum));
	}
	
	releaseTree(pTree);
	pTree = NULL;

	displayTree(pTree);

	printf("\n");
	printf("ok\n");
	
	getchar();
	return 0;
}
