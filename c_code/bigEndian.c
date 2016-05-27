

//小端：低位放在低地址  inter cpu是小端
//0x12345678  
//78 56 34 12

bool isBigEndian()
{
	union w
	{
		int a;
		char b;
	}c;

	//记住：一定是给a赋值，判断b；如果设b=2，判断a会出错，因为没有初始化，所以a中第二三四字节不为0
	c.a = 1;
	return (c.b != 1);
}


//测试两点：
//一、栈空间分配地址是递减的；a地址比b地址高.
//二、大小端
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

//x的值为： 0xcccc1234