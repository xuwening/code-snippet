
//求buf的偏移地址
typedef struct  
{
	char c;
	int a;
	char buf[10];
}tts_t;

#define  addr_tts_t_buf (int)(&((tts_t *)0)->buf)



//对齐，d为大小，a为对齐字节数
// ~(a-1) : a大小对应的位均为0
//(d) + (a - 1) : 比d大的最小的数
//相与: 求出d为参照的a的倍数
#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))