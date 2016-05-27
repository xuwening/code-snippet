
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

/*
scanf��ʽ��
format������һ������ {% [{h | l | I64 | L}]type | ' ' | '\t' | '\n' | ��%����} 

% [*] [size] [l] [h] type

  [] ֻ�����������ַ�
  [^] ������^���ַ�
  *  �����Ӧ�Ĳ������ݺ���
  l  ����Ϊlong int �� double
  h  short int
type %d  %i  %o %u %x %X %f %e %E %g %s %c 

  //��ʽʾ����
  // "%*d"  ����һ����������
  //

  "%4s"
  "%[^ ]"
  "%[1-9a-z]"
  "%[^A-Z]"
   "%*[^/]/%[^@]"
   "%*s%s"           //  *s����ո�
   "%[0-9,:] - %[0-9,:]"   //  %[]������һ��������ʽ

  */


//�������ڸ�ʽ
#define DATE_TEST
#define IDENTITY_CARD_TEST

#ifdef DATE_TEST
#define VAR_DATE y, m, d_
int VAR_DATE;
char *stringBuffer = "2011:02:09";
char *format = "%d:%d:%d";
#define SAVE_VAR &a, &m, &d_

#define GET_STRING_FORMAT_DATE() sscanf(stringBuffer, format, SAVE_VAR)
#define DISPLAY_VAR_DATE() printf(format, VAR_DATE)
#endif

/*  ��������
��	1	����	14
��	2	����	15
��	3	����	16
��	4	����	17
��	5	����	18
��	6	����	19
��	7	����	20
��	8	L	50
��	9	C	100
��	10	CL	150
��	11	CC	200
��	12	CCL	250
���� 13	CCC	300
*/

//  I - 1     V - 5      X - 10      L - 50      C - 100     D - 500     M - 1000
//��������û��0

//    6         8         3       1      = 18
//  xxxxxx - xxxxxxx - serial - verify
//  ������   ��������  ͬ�������  У����
//  ��ţ�2+1  1����Ϊ�У�żΪŮ
//  У���룺ISO 7064:1983.MOD 11-2У�������,  X�����������е�10

/*
15λ���֤�����λ�ĺ���: 
1-2λʡ����������ֱϽ�д��룻 
3-4λ�ؼ��С��ˡ������ݴ��룻 
5-6λ�ء��ؼ��С������룻 
7-12λ����������,����670401����1967��4��1��,���Ǻ�18λ����ĵ�һ������ 
13-15λΪ˳��ţ�����15λ��Ϊ������ŮΪ˫���� 
��18λ���֤�ŵĵڶ�������û�����һλ��У���롣

  ���֤��18λ��У���룩�ļ��㷽��
  ����1����ǰ������֤����17λ���ֱ���Բ�ͬ��ϵ�����ӵ�һλ����ʮ��λ��ϵ���ֱ�Ϊ��7��9��10��5��8��4��2��1��6��3��7��9��10��5��8��4��2��
  ����2������17λ���ֺ�ϵ����˵Ľ����ӡ�
  ����3���üӳ����ͳ���11���������Ƕ��٣�
  ����4������ֻ������0��1��2��3��4��5��6��7��8��9��10��11�����֡���ֱ��Ӧ�����һλ���֤�ĺ���Ϊ1��0��X ��9��8��7��6��5��4��3��2 ��
����5��ͨ�������֪���������2�����֤�����һλ���������������X�����������10���ͻ������֤�ĵ�18λ�����ϳ��ֵ���2��
*/

#ifdef IDENTITY_CARD_TEST
#define CARD_VAR a, b, c, d, e, f
int CARD_VAR;
char ch;

//��������:17λ
int factor[] = {
	7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2
};
//��������
int divided_factor = 11;
//��Ӧ����
char result_factor[] = {
	'1','0','X' ,'9','8','7','6','5','4','3','2'
};

char *stringBuffer_card = "610113197901248373";

//  xxxxxx - xxxxxxx - serial - verify
char *format_card = "%6d  %4d  %2d  %2d  %2d  %1d  %c";
#define SAVE_VAR &a, &b, &c, &d, &e, &f, &ch

#define GET_STRING_FORMAT_CARD() sscanf(stringBuffer_card, format_card, SAVE_VAR)
#define DISPLAY_VAR_CARD() printf(format_card, CARD_VAR, ch)

//У��λ�㷨��
//���������֤ǰ17λ
//�����У��λ�ַ�
char verify_iso_7064(int *card_array)
{
	int i;
	unsigned long count = 0;
	
	for (i=0; i<17; i++)
	{
		//ÿλ�����Գ�������
		card_array[i] *= factor[i];

		//�ۼ�
		count += card_array[i];
	}

	count %= divided_factor;
	return result_factor[count];
}

#endif


//����
int main()
{
	char buf[100];
	memset(buf, 0, sizeof(buf));
	sscanf("hello, world", "%*s%s", buf); 
	printf("%s", buf);


#ifdef DATE_TEST
	GET_STRING_FORMAT_DATE();
	DISPLAY_VAR_DATE();
#endif

	printf("\n");

#ifdef IDENTITY_CARD_TEST
	GET_STRING_FORMAT_CARD();
	DISPLAY_VAR_CARD();

	printf("\n");

	{ //У��λ����
		char ch;
		int card_num_seri[] = {6,1,0,1,1,3,1,9,7,9,0,1,2,4,8,3,7};
		ch = verify_iso_7064(card_num_seri);
		printf("verify ch : %c \n", ch);
	}
#endif


	getchar();
	return 0;
}