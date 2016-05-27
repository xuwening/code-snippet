#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>

//================最大公约数===================

//辗转相除法,欧几里得算法
int gcd(int a, int b)
{
	if (a == 0) return b;
	if (b == 0) return a;
	return gcd(b, a%b);
}

//Stein算法
int stein_gcd(int a, int b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    if (a % 2 == 0 && b % 2 == 0) return 2 * gcd(a/2,b/2);
    else if (a % 2 == 0) return gcd(a/2,b);
    else if (b % 2 == 0) return gcd(a,b/2);
    else return gcd(abs(a-b),min(a,b));
}

//================最小公倍数===================

//最大公约数 * 最小公倍数 = 两数乘积
//所以最小公倍数转换为求最大公约数，然后用两数乘积想除
int lcd(int a, int b)
{
	return (a * b) / gcd(a, b);
}


//测试
int main()
{	
	printf("%d\n", gcd(4, 6));

	printf("%d \n", lcd(4, 6));


	getchar();
	return 0;
}
