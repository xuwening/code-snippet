#include <stdlib.h>
#include <string.h>

//变参实现
//========================================================
typedef char * my_va_list;

//按照int对齐，参数堆栈int对齐
#define align_int(n) ( (sizeof(n) + (sizeof(int) - 1)) & ~(sizeof(int) - 1) )

#define my_va_start(pstart, val) (pstart = ((my_va_list)&val + align_int(val)))
//地址自加，然后取原来地址的数据
#define my_va_arg(pstart, type) (*(type *)((pstart += align_int(type)) - align_int(type)))
#define my_va_end(pstart) (pstart = (my_va_list)0)
//========================================================


//不定参数用法
void my_printf(char *fmt, ...)
{
	int i = 0;
	short s[] = {0, 0};
	char ch[] = {0, 0};
	
	char *pString = NULL;
	
	
	my_va_list args;
	
	my_va_start(args, fmt);
	
	//========================================================
	if (pString = my_va_arg(args, char *))
		printf("index: %i------>%s\n", i++, pString);
	if (pString = my_va_arg(args, char *))
		printf("index: %i------>%s\n", i++, pString);
	if (pString = my_va_arg(args, char *))
		printf("index: %i------>%s\n", i++, pString);
	
	ch[0] = my_va_arg(args, char);
	i = my_va_arg(args, int);
	s[0] = my_va_arg(args, short);
	ch[1] = my_va_arg(args, char);
	s[1] = my_va_arg(args, short);
	
	printf("%c---%d---%d---%c---%d\n", ch[0], i, s[0], ch[1], s[1]);
	//========================================================
	
	//其他用法： vprintf  vsprintf   vfprintf
	//他们的参数都是va_list，这样就实现了嵌套printf使用，即，变参可以传递到子函数中
	vprintf(fmt, args);
	
	
	my_va_end(args);
}



//测试
int main()
{
	my_printf("%s", "111", "222", "333", 'c', 100, 50, 'b', 50, 0000, 0000);
	getchar();
	return 0;
}