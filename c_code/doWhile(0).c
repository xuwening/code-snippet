
//最安全的宏写法
#define SAFE_FREE(P)  do {free(P); P=NULL;} while(0)