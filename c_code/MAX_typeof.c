//------linux-2.6.21/include/linux/kernel.h-------
//����:
//max(a++, b++)
//max(2, 44.8)

#define max(x,y) ({ \
    typeof(x) _x = (x);    \
    typeof(y) _y = (y);    \
    (void) (&_x == &_y);        \		//a,b��ͬ��������ʱ�����뾯��
    _x > _y ? _x : _y; })
	
	
//����> < if ��䣬ʵ��max�궨��
#define my_max(a,b) (((a)-(b))&(1<<31))?(b):(a) 