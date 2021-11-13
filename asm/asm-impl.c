#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm (
    "addq %[b], %0"
    :"=r"(a)
    :"0"(a), [b] "r"(b)
  );
  return a;
}

int asm_popcnt(uint64_t x) {
  int64_t s = 0;
  asm (
    "popcntq %1, %0"
    :"=r"(s)
    :"r"(x)
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm (
    ".byte 0xf3, 0xa4" //"rep; movsb"
    :
    :"c"(n)
    :"%rsi", "%rdi", "memory"
  );
  /*
  asm (
    "  mov $0x0, %%rax;"
		".loop2:;"
		"  cmp %%rbx, %%rax;"
		"  jge .ret2;"
		"  movb (%%rdx), %%cl;"
		"  movb %%cl, (%%rsi);"
		"  add $0x1, %%rsi;"
		"  add $0x1, %%rdx;"
		"  add $0x1, %%rax;"
		"  jmp .loop2;"
		".ret2:;"
		: 
		: "b"(n), "S"(dest), "d"(src)
		: "%cl", "%rax"
  );
  */
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
