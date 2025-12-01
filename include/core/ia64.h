#ifndef __CORE_IA64_H__
#define __CORE_IA64_H__

#include <core/types.h>

#define EFLAGS_CF   0x0001
#define EFLAGS_PF   0x0004
#define EFLAGS_AF   0x0010
#define EFLAGS_ZF   0x0040
#define EFLAGS_SF   0x0080
#define EFLAGS_TF   0x0100
#define EFLAGS_IF   0x0200
#define EFLAGS_DF   0x0400
#define EFLAGS_OF   0x0800
#define EFLAGS_IOPL 0x3000
#define EFLAGS_NT   0x4000
#define EFLAGS_MD   0x8000
#define EFLAGS_RF   0x00010000
#define EFLAGS_VM   0x00020000
#define EFLAGS_AC   0x00040000
#define EFLAGS_VIF  0x00080000
#define EFLAGS_VIP  0x00100000
#define EFLAGS_ID   0x00200000
#define EFLAGS_AI   0x80000000

struct x64_gpr {
    u64 rax;
    u64 rbx;
    u64 rcx;
    u64 rdx;
    u64 rsi;
    u64 rdi;
    u64 rbp;
    u64 rsp;
    u64 r8;
    u64 r9;
    u64 r10;
    u64 r11;
    u64 r12;
    u64 r13;
    u64 r14;
    u64 r15;
};

#define M_FENCE()   asm volatile ("": : :"memory")


enum cpuid_leaves {
    CPUID_FUNC_VENDOR_ID = 0,
};
b32 x64_has_cpuid(void);
void x64_cpuid(struct x64_gpr *regs);

#endif /* __CORE_IA64_H__ */
