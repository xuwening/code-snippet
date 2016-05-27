
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

//"#" 代表和一个字符串相连接 

//"##" 代表和一个符号连接，符号可以是变量，或另一个宏符号。 

//单独一个#，则表示对这个变量替换后，再加双引号引起来
#define flag_test(a) #a

//##后的参数本身也是一个宏的话，##会阻止这个宏的展开，也就是只替换一次。
//记号粘贴操作符(token paste operator): ##
//如果##后的参数本身也是一个宏的话，##会阻止这个宏的展开 
#define ANDSTRING(a, b) my##b


#define A1(name, type)  type name_##type##_type
#define A2(name, type)  type name##_##type##_type

A1(a1, int);  /* 等价于: int name_int_type; */
A2(a1, int);  /* 等价于: int a1_int_type;   */
//测试
int main()
{
	printf("%s\n", flag_test("3"));

	printf("%s\n", ANDSTRING(a_string, b_string));
	getchar();
	return 0;
}