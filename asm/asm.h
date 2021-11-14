#include <stdint.h>
#include <stddef.h>


typedef long asm_jmp_buf[8]; // rbx, rbp, rsp, r12, r13, r14, r15, pc

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     asm_setjmp(asm_jmp_buf env);
void    asm_longjmp(asm_jmp_buf env, int val);
