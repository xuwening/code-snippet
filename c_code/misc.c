
//����δʹ�ñ����ľ���
int i;
(void)i;


//�ж� a == 0 && b == 0 && c == 0 ...
( (a | b | c) == 0 );

//(a == 0 && b != 0) || ( a != 0 && b == 0)
( (!a) ^ (!b) );

//����a �� b
a ^= b;
b ^= a;
a ^= b;


//���á��������תĳλ
a = (a | (1L << i) );
a = ( a & ~(1L << i) );
a = ( a ^ (1L << i) );



//�ṹ����
//�ѵ�ǰ�����趨���棬����1�ֽڶ���
#pragma pack(push, 1)

//�����ϴζ����趨
#pragma pack(pop)



//����һ�����ͣ�����8��char������
typedef char array_t[8];
array_t ch_buf;


//�ж�һ�����Ƿ���2��n�η�
if ((x & (x-1))  == 0)


//ʹ�ò�ռ�ռ������ṹ
typedef struct {
	
	int a;
	int b;
	char [];
}test_t;


//��ȡĳ�ṹ��Աƫ����
typedef struct  
{
	int age;
	char ch;
	char buffer[];
}test_struct_t;

int len = &((test_struct_t *)0)->buffer;


//�ļ����ȣ����ַ�ʽ�ĺô��ǿ�ƽ̨
fseek(input, 0, SEEK_END);
len = ftell(input);
fseek(input, 0, SEEK_SET);


//����crush ���ڱ�Ҫʱ��������
#define MY_CRUSH(cond) do {if (!(cond)) ((void (*)())0)();} while (0)


//C Ԥ�����
__DATE__  //����Ԥ��������ڣ���Mmm dd yyyy����ʽ���ַ������֣�
__FILE__ //����ǰԴ�����ļ������ַ�������
__LINE__ //����ǰԴ�����е��кŵ���������
__TIME__ //Դ�ļ�����ʱ�䣬��ʽ΢��hh��mm��ss��
__func__ //��ǰ���ں����� 
printf("file: %s, line: %d, date: %s, time: %s\n", __FILE__, __LINE__, __DATE__, __TIME__);



