
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//��ұ����������ֽ�ɶ�����������
//��ȱ�㣺�������ⲻ��������͸�����---����ʹ�������Ķ�̬�滮����
int fibonacci(int a)
{
	if (a == 1 || a == 2)
		return 1;
	return fibonacci(a-1) + fibonacci(a-2);
}

//�Ե����϶�̬�滮��
//ͨ�����㰴�����еĵ����к���ֵ������С��ʼ����ÿһ��ʹ��ǰһ������õ���ֵ
//�����㵱ǰֵ���������Լ����κ���һ���͵ĺ�����
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

//�Զ����¶�̬�滮��
//���򵥵ļ��������Ե�������ȣ����ܹ���һ���Զ�����ʽ����ͬ���Ŀ�����ִ�еݹ麯��
//������ݹ���򱣴����������ÿ��ֵ����Ϊ������������Ҽ���������ֵ��
//�������¼������ǡ�
void dynamic_fibonacci_updown(int *buffer, int index)
{
	if (index == 0 || index == 1)
		return;

	dynamic_fibonacci_updown(buffer, index-1);
	//dynamic_fibonacci_updown(buffer, index-2);
	buffer[index] = buffer[index-1] + buffer[index-2];
}


//=============================================================
//����
//=============================================================
void main(void)
{
	int i;

	int buffer[100];
	memset(buffer, 0, sizeof(buffer));

	//�Ե����ϣ���̬�滮��
	dynamic_fibonacci_downup(buffer, sizeof(buffer)/sizeof(buffer[0]));
	for (i=0; i<sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		printf("%d  ", buffer[i]);
	}

	printf("\n--------------------------\n");

	memset(buffer, 0, sizeof(buffer));
	buffer[0] = 0;
	buffer[1] = 1;

	//�Զ����£���̬�滮��
	dynamic_fibonacci_updown(buffer, 99);

	for (i=1; i<sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		printf("%d  ", buffer[i]);
	}

	//��ұ��
/*
	for (i=1; i<100; i++)
	{
		printf("%d  ", fibonacci(i));
	}
*/
	getchar();
}

