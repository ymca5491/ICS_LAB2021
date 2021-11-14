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
  /*
  asm (
    //".byte 0xf3, 0xa4" 
    "rep; movsb"
    :
    :"c"(n)
    :"%rsi", "%rdi", "memory"
  );
  */
  uint64_t t1, t2, t3;
  asm volatile (
    ".loop_memcpy:;"
    "   movb (%[src]), %%al;"
    "   movb %%al, (%[dst]);"
    "   addq $1, %[src];"
    "   addq $1, %[dst];"
    "   subq $1, %[n];"
    "   cmpq $0, %[n];"
    "   jg .loop_memcpy"
    :[src] "=&r"(t1), [dst] "=&r"(t2), [n] "=&r"(t3)
    :"0"(src), "1"(dest), "2"(n)
    :"%al", "memory"
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  asm (
    "movq %%rbx,    (%[buf]);"    // rbx
    "movq %%rbp,    8(%[buf]);"   // rbp
    "leaq 8(%%rsp), %%rax;"       // rsp (before call)
    "mov  %%rax,    16(%[buf]);"
    "movq %%r12,    24(%[buf]);"  // r12
    "movq %%r13,    32(%[buf]);"  // r13
    "movq %%r14,    40(%[buf]);"  // r14
    "movq %%r15,    48(%[buf]);"  // r15
    "movq (%%rsp),  %%rax;"       // (%esp) == pc
    "movq %%rax,    56(%[buf]);"
    :
    :[buf] "r"(env)
    :"memory", "%rax"
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm (
    "movq (%[buf]),     %%rbx;"    // rbx
    "movq 8(%[buf]),    %%rbp;"    // rbp
    "movq 16(%[buf],    %%rsp;"    // rsp
    "movq 24(%[buf]),   %%r12;"    // r12
    "movq 32(%[buf]),   %%r13;"    // r13
    "movq 40(%[buf]),   %%r14;"    // r14
    "movq 48(%[buf]),   %%r15;"    // r15
    "jmpq 56(%[buf])"              // jmp
    :
    :[buf] "r"(env), "a"(val)
  );
}
