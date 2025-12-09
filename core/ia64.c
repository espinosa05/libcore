#include <core/memory.h>
#include <core/ia64.h>
#include <core/asm.h>

b32 x64_has_cpuid(void)
{
    u64 flags = 0;

    asm volatile (
        ASM_STMT("pushfq")
        ASM_STMT("popq %0")
        : "=r" (flags)
    );

    /* check if id bit is present */
    return EFLAGS_ID == (flags & EFLAGS_ID);
}

void x64_cpuid(struct x64_gpr *regs)
{
    M_FENCE();
    asm volatile (
        ASM_STMT("cpuid")
        : "=a" (regs->rax), "=b" (regs->rbx), "=c" (regs->rcx), "=d" (regs->rdx)
        : "a"  (regs->rax), "c"(regs->rcx)
    );
}

