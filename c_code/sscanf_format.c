
#include "StdAfx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

/*
scanf格式：
format可以是一个或多个 {% [{h | l | I64 | L}]type | ' ' | '\t' | '\n' | 非%符号} 

% [*] [size] [l] [h] type

  [] 只允许括号内字符
  [^] 不允许^后字符
  *  代表对应的参数数据忽略
  l  数据为long int 或 double
  h  short int
type %d  %i  %o %u %x %X %f %e %E %g %s %c 

  //格式示例：
  // "%*d"  跳过一个整数不读
  //

  "%4s"
  "%[^ ]"
  "%[1-9a-z]"
  "%[^A-Z]"
   "%*[^/]/%[^@]"
   "%*s%s"           //  *s不算空格
   "%[0-9,:] - %[0-9,:]"   //  %[]类似于一个正则表达式

  */


//测试日期格式
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

/*  罗马数字
Ⅰ	1	ⅩⅣ	14
Ⅱ	2	ⅩⅤ	15
Ⅲ	3	ⅩⅥ	16
Ⅳ	4	ⅩⅦ	17
Ⅴ	5	ⅩⅧ	18
Ⅵ	6	ⅩⅨ	19
Ⅶ	7	ⅩⅩ	20
Ⅷ	8	L	50
Ⅸ	9	C	100
Ⅹ	10	CL	150
Ⅺ	11	CC	200
Ⅻ	12	CCL	250
ⅩⅢ 13	CCC	300
*/

//  I - 1     V - 5      X - 10      L - 50      C - 100     D - 500     M - 1000
//罗马数字没有0

//    6         8         3       1      = 18
//  xxxxxx - xxxxxxx - serial - verify
//  县区码   出生日期  同生日序号  校验码
//  序号：2+1  1：奇为男，偶为女
//  校验码：ISO 7064:1983.MOD 11-2校验码计算,  X是罗马数字中的10

/*
15位身份证号码各位的含义: 
1-2位省、自治区、直辖市代码； 
3-4位地级市、盟、自治州代码； 
5-6位县、县级市、区代码； 
7-12位出生年月日,比如670401代表1967年4月1日,这是和18位号码的第一个区别； 
13-15位为顺序号，其中15位男为单数，女为双数； 
与18位身份证号的第二个区别：没有最后一位的校验码。

  身份证第18位（校验码）的计算方法
  　　1、将前面的身份证号码17位数分别乘以不同的系数。从第一位到第十七位的系数分别为：7－9－10－5－8－4－2－1－6－3－7－9－10－5－8－4－2。
  　　2、将这17位数字和系数相乘的结果相加。
  　　3、用加出来和除以11，看余数是多少？
  　　4、余数只可能有0－1－2－3－4－5－6－7－8－9－10这11个数字。其分别对应的最后一位身份证的号码为1－0－X －9－8－7－6－5－4－3－2 。
　　5、通过上面得知如果余数是2，身份证的最后一位号码就是罗马数字X。如果余数是10，就会在身份证的第18位数字上出现的是2。
*/

#ifdef IDENTITY_CARD_TEST
#define CARD_VAR a, b, c, d, e, f
int CARD_VAR;
char ch;

//乘数因子:17位
int factor[] = {
	7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2
};
//余数因子
int divided_factor = 11;
//对应因子
char result_factor[] = {
	'1','0','X' ,'9','8','7','6','5','4','3','2'
};

char *stringBuffer_card = "610113197901248373";

//  xxxxxx - xxxxxxx - serial - verify
char *format_card = "%6d  %4d  %2d  %2d  %2d  %1d  %c";
#define SAVE_VAR &a, &b, &c, &d, &e, &f, &ch

#define GET_STRING_FORMAT_CARD() sscanf(stringBuffer_card, format_card, SAVE_VAR)
#define DISPLAY_VAR_CARD() printf(format_card, CARD_VAR, ch)

//校验位算法：
//参数：身份证前17位
//结果：校验位字符
char verify_iso_7064(int *card_array)
{
	int i;
	unsigned long count = 0;
	
	for (i=0; i<17; i++)
	{
		//每位数乘以乘数因子
		card_array[i] *= factor[i];

		//累计
		count += card_array[i];
	}

	count %= divided_factor;
	return result_factor[count];
}

#endif


//测试
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

	{ //校验位测试
		char ch;
		int card_num_seri[] = {6,1,0,1,1,3,1,9,7,9,0,1,2,4,8,3,7};
		ch = verify_iso_7064(card_num_seri);
		printf("verify ch : %c \n", ch);
	}
#endif


	getchar();
	return 0;
}