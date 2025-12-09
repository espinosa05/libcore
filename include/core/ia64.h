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

/* this was done by ChatGPT ... that was just too tedious */
#define X64_GPR_RAX(gpr)    ((gpr).rax)
#define X64_GPR_RBX(gpr)    ((gpr).rbx)
#define X64_GPR_RCX(gpr)    ((gpr).rcx)
#define X64_GPR_RDX(gpr)    ((gpr).rdx)
#define X64_GPR_RSI(gpr)    ((gpr).rsi)
#define X64_GPR_RDI(gpr)    ((gpr).rdi)
#define X64_GPR_RBP(gpr)    ((gpr).rbp)
#define X64_GPR_RSP(gpr)    ((gpr).rsp)

#define X64_GPR_R8(gpr)     ((gpr).r8)
#define X64_GPR_R9(gpr)     ((gpr).r9)
#define X64_GPR_R10(gpr)    ((gpr).r10)
#define X64_GPR_R11(gpr)    ((gpr).r11)
#define X64_GPR_R12(gpr)    ((gpr).r12)
#define X64_GPR_R13(gpr)    ((gpr).r13)
#define X64_GPR_R14(gpr)    ((gpr).r14)
#define X64_GPR_R15(gpr)    ((gpr).r15)

#define X64_GPR_EAX(gpr)    (u32)((gpr).rax)
#define X64_GPR_EBX(gpr)    (u32)((gpr).rbx)
#define X64_GPR_ECX(gpr)    (u32)((gpr).rcx)
#define X64_GPR_EDX(gpr)    (u32)((gpr).rdx)
#define X64_GPR_ESI(gpr)    (u32)((gpr).rsi)
#define X64_GPR_EDI(gpr)    (u32)((gpr).rdi)
#define X64_GPR_EBP(gpr)    (u32)((gpr).rbp)
#define X64_GPR_ESP(gpr)    (u32)((gpr).rsp)

#define X64_GPR_R8D(gpr)    (u32)((gpr).r8)
#define X64_GPR_R9D(gpr)    (u32)((gpr).r9)
#define X64_GPR_R10D(gpr)   (u32)((gpr).r10)
#define X64_GPR_R11D(gpr)   (u32)((gpr).r11)
#define X64_GPR_R12D(gpr)   (u32)((gpr).r12)
#define X64_GPR_R13D(gpr)   (u32)((gpr).r13)
#define X64_GPR_R14D(gpr)   (u32)((gpr).r14)
#define X64_GPR_R15D(gpr)   (u32)((gpr).r15)

#define X64_GPR_AX(gpr)     (u16)((gpr).rax)
#define X64_GPR_BX(gpr)     (u16)((gpr).rbx)
#define X64_GPR_CX(gpr)     (u16)((gpr).rcx)
#define X64_GPR_DX(gpr)     (u16)((gpr).rdx)
#define X64_GPR_SI(gpr)     (u16)((gpr).rsi)
#define X64_GPR_DI(gpr)     (u16)((gpr).rdi)
#define X64_GPR_BP(gpr)     (u16)((gpr).rbp)
#define X64_GPR_SP(gpr)     (u16)((gpr).rsp)

#define X64_GPR_R8W(gpr)    (u16)((gpr).r8)
#define X64_GPR_R9W(gpr)    (u16)((gpr).r9)
#define X64_GPR_R10W(gpr)   (u16)((gpr).r10)
#define X64_GPR_R11W(gpr)   (u16)((gpr).r11)
#define X64_GPR_R12W(gpr)   (u16)((gpr).r12)
#define X64_GPR_R13W(gpr)   (u16)((gpr).r13)
#define X64_GPR_R14W(gpr)   (u16)((gpr).r14)
#define X64_GPR_R15W(gpr)   (u16)((gpr).r15)

#define X64_GPR_AH(gpr)     (u8)(((gpr).rax >> 8) & 0xFFU)
#define X64_GPR_BH(gpr)     (u8)(((gpr).rbx >> 8) & 0xFFU)
#define X64_GPR_CH(gpr)     (u8)(((gpr).rcx >> 8) & 0xFFU)
#define X64_GPR_DH(gpr)     (u8)(((gpr).rdx >> 8) & 0xFFU)

#define X64_GPR_AL(gpr)     (u8)((gpr).rax)
#define X64_GPR_BL(gpr)     (u8)((gpr).rbx)
#define X64_GPR_CL(gpr)     (u8)((gpr).rcx)
#define X64_GPR_DL(gpr)     (u8)((gpr).rdx)
#define X64_GPR_R8B(gpr)    (u8)((gpr).r8)
#define X64_GPR_R9B(gpr)    (u8)((gpr).r9)
#define X64_GPR_R10B(gpr)   (u8)((gpr).r10)
#define X64_GPR_R11B(gpr)   (u8)((gpr).r11)
#define X64_GPR_R12B(gpr)   (u8)((gpr).r12)
#define X64_GPR_R13B(gpr)   (u8)((gpr).r13)
#define X64_GPR_R14B(gpr)   (u8)((gpr).r14)
#define X64_GPR_R15B(gpr)   (u8)((gpr).r15)

#define X64_GPR_SET_RAX(gpr, v)     ((gpr)->rax = (u64)(v))
#define X64_GPR_SET_RBX(gpr, v)     ((gpr)->rbx = (u64)(v))
#define X64_GPR_SET_RCX(gpr, v)     ((gpr)->rcx = (u64)(v))
#define X64_GPR_SET_RDX(gpr, v)     ((gpr)->rdx = (u64)(v))
#define X64_GPR_SET_RSI(gpr, v)     ((gpr)->rsi = (u64)(v))
#define X64_GPR_SET_RDI(gpr, v)     ((gpr)->rdi = (u64)(v))
#define X64_GPR_SET_RBP(gpr, v)     ((gpr)->rbp = (u64)(v))
#define X64_GPR_SET_RSP(gpr, v)     ((gpr)->rsp = (u64)(v))

#define X64_GPR_SET_R8(gpr, v)      ((gpr)->r8  = (u64)(v))
#define X64_GPR_SET_R9(gpr, v)      ((gpr)->r9  = (u64)(v))
#define X64_GPR_SET_R10(gpr, v)     ((gpr)->r10 = (u64)(v))
#define X64_GPR_SET_R11(gpr, v)     ((gpr)->r11 = (u64)(v))
#define X64_GPR_SET_R12(gpr, v)     ((gpr)->r12 = (u64)(v))
#define X64_GPR_SET_R13(gpr, v)     ((gpr)->r13 = (u64)(v))
#define X64_GPR_SET_R14(gpr, v)     ((gpr)->r14 = (u64)(v))
#define X64_GPR_SET_R15(gpr, v)     ((gpr)->r15 = (u64)(v))

#define X64_GPR_SET_EAX(gpr, v)     ((gpr)->rax = (u64)(u32)(v))
#define X64_GPR_SET_EBX(gpr, v)     ((gpr)->rbx = (u64)(u32)(v))
#define X64_GPR_SET_ECX(gpr, v)     ((gpr)->rcx = (u64)(u32)(v))
#define X64_GPR_SET_EDX(gpr, v)     ((gpr)->rdx = (u64)(u32)(v))
#define X64_GPR_SET_ESI(gpr, v)     ((gpr)->rsi = (u64)(u32)(v))
#define X64_GPR_SET_EDI(gpr, v)     ((gpr)->rdi = (u64)(u32)(v))
#define X64_GPR_SET_EBP(gpr, v)     ((gpr)->rbp = (u64)(u32)(v))
#define X64_GPR_SET_ESP(gpr, v)     ((gpr)->rsp = (u64)(u32)(v))
#define X64_GPR_SET_R8D(gpr, v)     ((gpr)->r8  = (u64)(u32)(v))
#define X64_GPR_SET_R9D(gpr, v)     ((gpr)->r9  = (u64)(u32)(v))
#define X64_GPR_SET_R10D(gpr, v)    ((gpr)->r10 = (u64)(u32)(v))
#define X64_GPR_SET_R11D(gpr, v)    ((gpr)->r11 = (u64)(u32)(v))
#define X64_GPR_SET_R12D(gpr, v)    ((gpr)->r12 = (u64)(u32)(v))
#define X64_GPR_SET_R13D(gpr, v)    ((gpr)->r13 = (u64)(u32)(v))
#define X64_GPR_SET_R14D(gpr, v)    ((gpr)->r14 = (u64)(u32)(v))
#define X64_GPR_SET_R15D(gpr, v)    ((gpr)->r15 = (u64)(u32)(v))

#define X64_GPR_SET_AX(gpr, v)      ((gpr)->rax = ((gpr)->rax & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_BX(gpr, v)      ((gpr)->rbx = ((gpr)->rbx & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_CX(gpr, v)      ((gpr)->rcx = ((gpr)->rcx & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_DX(gpr, v)      ((gpr)->rdx = ((gpr)->rdx & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_SI(gpr, v)      ((gpr)->rsi = ((gpr)->rsi & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_DI(gpr, v)      ((gpr)->rdi = ((gpr)->rdi & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_BP(gpr, v)      ((gpr)->rbp = ((gpr)->rbp & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_SP(gpr, v)      ((gpr)->rsp = ((gpr)->rsp & ~0xFFFFULL) | (u64)((u16)(v)))

#define X64_GPR_SET_R8W(gpr, v)     ((gpr)->r8  = ((gpr)->r8  & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R9W(gpr, v)     ((gpr)->r9  = ((gpr)->r9  & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R10W(gpr, v)    ((gpr)->r10 = ((gpr)->r10 & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R11W(gpr, v)    ((gpr)->r11 = ((gpr)->r11 & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R12W(gpr, v)    ((gpr)->r12 = ((gpr)->r12 & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R13W(gpr, v)    ((gpr)->r13 = ((gpr)->r13 & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R14W(gpr, v)    ((gpr)->r14 = ((gpr)->r14 & ~0xFFFFULL) | (u64)((u16)(v)))
#define X64_GPR_SET_R15W(gpr, v)    ((gpr)->r15 = ((gpr)->r15 & ~0xFFFFULL) | (u64)((u16)(v)))

#define X64_GPR_SET_AH(gpr, v)      ((gpr)->rax = ((gpr)->rax & ~(0xFFULL << 8)) | ((u64)((u8)(v)) << 8))
#define X64_GPR_SET_BH(gpr, v)      ((gpr)->rbx = ((gpr)->rbx & ~(0xFFULL << 8)) | ((u64)((u8)(v)) << 8))
#define X64_GPR_SET_CH(gpr, v)      ((gpr)->rcx = ((gpr)->rcx & ~(0xFFULL << 8)) | ((u64)((u8)(v)) << 8))
#define X64_GPR_SET_DH(gpr, v)      ((gpr)->rdx = ((gpr)->rdx & ~(0xFFULL << 8)) | ((u64)((u8)(v)) << 8))

#define X64_GPR_SET_AL(gpr, v)      ((gpr)->rax = ((gpr)->rax & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_BL(gpr, v)      ((gpr)->rbx = ((gpr)->rbx & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_CL(gpr, v)      ((gpr)->rcx = ((gpr)->rcx & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_DL(gpr, v)      ((gpr)->rdx = ((gpr)->rdx & ~0xFFULL) | (u64)((u8)(v)))

#define X64_GPR_SET_R8B(gpr, v)     ((gpr)->r8  = ((gpr)->r8  & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R9B(gpr, v)     ((gpr)->r9  = ((gpr)->r9  & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R10B(gpr, v)    ((gpr)->r10 = ((gpr)->r10 & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R11B(gpr, v)    ((gpr)->r11 = ((gpr)->r11 & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R12B(gpr, v)    ((gpr)->r12 = ((gpr)->r12 & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R13B(gpr, v)    ((gpr)->r13 = ((gpr)->r13 & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R14B(gpr, v)    ((gpr)->r14 = ((gpr)->r14 & ~0xFFULL) | (u64)((u8)(v)))
#define X64_GPR_SET_R15B(gpr, v)    ((gpr)->r15 = ((gpr)->r15 & ~0xFFULL) | (u64)((u8)(v)))

enum cpuid_leaves {
    CPUID_FUNC_VENDOR_ID = 0,
};
b32 x64_has_cpuid(void);
void x64_cpuid(struct x64_gpr *regs);

#endif /* __CORE_IA64_H__ */
