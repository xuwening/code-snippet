#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>

//================���Լ��===================

//շת�����,ŷ������㷨
int gcd(int a, int b)
{
	if (a == 0) return b;
	if (b == 0) return a;
	return gcd(b, a%b);
}

//Stein�㷨
int stein_gcd(int a, int b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    if (a % 2 == 0 && b % 2 == 0) return 2 * gcd(a/2,b/2);
    else if (a % 2 == 0) return gcd(a/2,b);
    else if (b % 2 == 0) return gcd(a,b/2);
    else return gcd(abs(a-b),min(a,b));
}

//================��С������===================

//���Լ�� * ��С������ = �����˻�
//������С������ת��Ϊ�����Լ����Ȼ���������˻����
int lcd(int a, int b)
{
	return (a * b) / gcd(a, b);
}


//����
int main()
{	
	printf("%d\n", gcd(4, 6));

	printf("%d \n", lcd(4, 6));


	getchar();
	return 0;
}
