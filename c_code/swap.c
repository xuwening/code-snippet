//无变量交换，均有缺陷，不合适，所以还是应该用变量交换

inline void swap(int &a, int &b)
{
　　a = a + b; //缺陷1: 可能产生上溢.
　　b = a - b;
　　a = a - b;
}
// 缺陷 2: 传入同一变量, 变量结果为 0.
swap(a, a);




inline void swap(const &a, const &b)
{
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}
