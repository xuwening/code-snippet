
//避免未使用变量的警告
int i;
(void)i;


//判断 a == 0 && b == 0 && c == 0 ...
( (a | b | c) == 0 );

//(a == 0 && b != 0) || ( a != 0 && b == 0)
( (!a) ^ (!b) );

//交换a 和 b
a ^= b;
b ^= a;
a ^= b;


//设置、清除、翻转某位
a = (a | (1L << i) );
a = ( a & ~(1L << i) );
a = ( a ^ (1L << i) );



//结构对齐
//把当前对齐设定保存，并按1字节对齐
#pragma pack(push, 1)

//返回上次对齐设定
#pragma pack(pop)



//定义一个类型，包含8个char的数组
typedef char array_t[8];
array_t ch_buf;


//判断一个数是否是2的n次方
if ((x & (x-1))  == 0)


//使用不占空间的数组结构
typedef struct {
	
	int a;
	int b;
	char [];
}test_t;


//获取某结构成员偏移量
typedef struct  
{
	int age;
	char ch;
	char buffer[];
}test_struct_t;

int len = &((test_struct_t *)0)->buffer;


//文件长度，这种方式的好处是跨平台
fseek(input, 0, SEEK_END);
len = ftell(input);
fseek(input, 0, SEEK_SET);


//定义crush ，在必要时引起重视
#define MY_CRUSH(cond) do {if (!(cond)) ((void (*)())0)();} while (0)


//C 预定义宏
__DATE__  //进行预处理的日期（“Mmm dd yyyy”形式的字符串文字）
__FILE__ //代表当前源代码文件名的字符串文字
__LINE__ //代表当前源代码中的行号的整数常量
__TIME__ //源文件编译时间，格式微“hh：mm：ss”
__func__ //当前所在函数名 
printf("file: %s, line: %d, date: %s, time: %s\n", __FILE__, __LINE__, __DATE__, __TIME__);



