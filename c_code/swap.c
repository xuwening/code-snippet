//�ޱ�������������ȱ�ݣ������ʣ����Ի���Ӧ���ñ�������

inline void swap(int &a, int &b)
{
����a = a + b; //ȱ��1: ���ܲ�������.
����b = a - b;
����a = a - b;
}
// ȱ�� 2: ����ͬһ����, �������Ϊ 0.
swap(a, a);




inline void swap(const &a, const &b)
{
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}
