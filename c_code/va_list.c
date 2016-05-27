#include <stdlib.h>
#include <string.h>

//���ʵ��
//========================================================
typedef char * my_va_list;

//����int���룬������ջint����
#define align_int(n) ( (sizeof(n) + (sizeof(int) - 1)) & ~(sizeof(int) - 1) )

#define my_va_start(pstart, val) (pstart = ((my_va_list)&val + align_int(val)))
//��ַ�Լӣ�Ȼ��ȡԭ����ַ������
#define my_va_arg(pstart, type) (*(type *)((pstart += align_int(type)) - align_int(type)))
#define my_va_end(pstart) (pstart = (my_va_list)0)
//========================================================


//���������÷�
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
	
	//�����÷��� vprintf  vsprintf   vfprintf
	//���ǵĲ�������va_list��������ʵ����Ƕ��printfʹ�ã�������ο��Դ��ݵ��Ӻ�����
	vprintf(fmt, args);
	
	
	my_va_end(args);
}



//����
int main()
{
	my_printf("%s", "111", "222", "333", 'c', 100, 50, 'b', 50, 0000, 0000);
	getchar();
	return 0;
}