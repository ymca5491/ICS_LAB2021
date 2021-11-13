#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm volatile (
    "addq %1, %0"
    :"=r"(a)
    :"r"(a), "r"(b)
  );
  return a;
}

int asm_popcnt(uint64_t x) {
  uint64_t s = 0;
  asm volatile (
    "popcntq %1, %0"
    :"=r"(s)
    :"r"(x)
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
