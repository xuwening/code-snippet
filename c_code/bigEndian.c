

//С�ˣ���λ���ڵ͵�ַ  inter cpu��С��
//0x12345678  
//78 56 34 12

bool isBigEndian()
{
	union w
	{
		int a;
		char b;
	}c;

	//��ס��һ���Ǹ�a��ֵ���ж�b�������b=2���ж�a�������Ϊû�г�ʼ��������a�еڶ������ֽڲ�Ϊ0
	c.a = 1;
	return (c.b != 1);
}


//�������㣺
//һ��ջ�ռ�����ַ�ǵݼ��ģ�a��ַ��b��ַ��.
//������С��
int main()
{
	int a = 0x12345678;
    int b = 0x567890AB;
    int c = 0x7890ABCD;
    
    char * p = (char*)&a;
    char * q = p + 2;
    int x = *(int*)q;
    
    printf("x=%X",x);
	return 0;
}

//x��ֵΪ�� 0xcccc1234