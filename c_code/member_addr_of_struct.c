
//��buf��ƫ�Ƶ�ַ
typedef struct  
{
	char c;
	int a;
	char buf[10];
}tts_t;

#define  addr_tts_t_buf (int)(&((tts_t *)0)->buf)



//���룬dΪ��С��aΪ�����ֽ���
// ~(a-1) : a��С��Ӧ��λ��Ϊ0
//(d) + (a - 1) : ��d�����С����
//����: ���dΪ���յ�a�ı���
#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))