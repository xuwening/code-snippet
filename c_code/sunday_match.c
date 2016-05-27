#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//sunday算法
//比BM算法更快，切容易理解。
//核心思想：在匹配过程中，模式串并不被要求一定
//要按从左向右进行比较还是从右向左进行比较，它在发现不匹配时，
//算法能跳过尽可能多的字符以进行下一步的匹配，从而提高了匹配效率。

//不匹配时的规则：查看T下个字符是否在P中出现
//1、未出现，则将P移动到下个字符的后一位
//2、出现，则将P和T按照该字符对齐

//eg: 这里e未在P中出现
//T :  a  b  c  e  a  b  d
//P :  a  b  d

//移动
//T :  a  b  c  e  a  b  d
//P :              a  b  d

//eg: 这里b在P中出现
//T :  a  b  c  b  a  b  d
//P :  a  b  d

//移动
//T :  a  b  c  b  a  b  d
//P :        a  b  d

//初始化next
void init_shift(int *next, int next_len, char *pattern, int pattern_len)
{
	int i;
	assert(next != NULL);
	for (i=0; i<next_len; i++)
	{
		next[i] = pattern_len+1;
	}

	for (i=0; i<pattern_len; i++)
	{
		next[pattern[i]-'a'] = pattern_len-i;
	}
}

//比较两字符串是否匹配, 0匹配，否则不匹配
int match_string(char *text, int text_len, char *pattern, int pattern_len)
{
	assert(text !=NULL && pattern != NULL);
	if (text_len < pattern_len)
		return -1;
	while (pattern_len--)
	{
		if (text[pattern_len] != pattern[pattern_len])
			return -1;
	}

	return 0;
}

//匹配返回0
int sunday_match(char *text, char *pattern)
{
	int ret = -1;
	int index = 0;
	int next['z'-'a'+1];
	int text_len = strlen(text);
	int pattern_len = strlen(pattern);

	assert(text != NULL && pattern != NULL);

	init_shift(next, sizeof(next)/sizeof(next[0]), pattern, pattern_len);

	while (index < text_len)
	{
		ret = match_string(text+index, text_len-index, pattern, pattern_len);
		if (ret != 0)
			index += next[text[index+pattern_len]-'a'];
		else
			break;
	}

	return ret;
}

int main(int argc, char *argv[])
{
    char src[]="abcdacdaahfacabcdabcdeaa";  
    char des[]="abcde"; 
	int ret;

	ret = sunday_match(src, des);
	printf("%d\n", ret);
	
	getchar();
	return 0;
}