
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

//"#" �����һ���ַ��������� 

//"##" �����һ���������ӣ����ſ����Ǳ���������һ������š� 

//����һ��#�����ʾ����������滻���ټ�˫����������
#define flag_test(a) #a

//##��Ĳ�������Ҳ��һ����Ļ���##����ֹ������չ����Ҳ����ֻ�滻һ�Ρ�
//�Ǻ�ճ��������(token paste operator): ##
//���##��Ĳ�������Ҳ��һ����Ļ���##����ֹ������չ�� 
#define ANDSTRING(a, b) my##b


#define A1(name, type)  type name_##type##_type
#define A2(name, type)  type name##_##type##_type

A1(a1, int);  /* �ȼ���: int name_int_type; */
A2(a1, int);  /* �ȼ���: int a1_int_type;   */
//����
int main()
{
	printf("%s\n", flag_test("3"));

	printf("%s\n", ANDSTRING(a_string, b_string));
	getchar();
	return 0;
}