//------linux-2.6.21/include/linux/kernel.h-------
//避免:
//max(a++, b++)
//max(2, 44.8)

#define max(x,y) ({ \
    typeof(x) _x = (x);    \
    typeof(y) _y = (y);    \
    (void) (&_x == &_y);        \		//a,b不同数据类型时，编译警告
    _x > _y ? _x : _y; })
	
	
//不用> < if 语句，实现max宏定义
#define my_max(a,b) (((a)-(b))&(1<<31))?(b):(a) 