#include <core/memory.h>
#include <core/ia64.h>
#include <core/asm.h>
#include <core/utils.h>

b32 x64_has_cpuid(void)
{
    M_FENCE();
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

#define X64_BRAND_STRING_PART_LENGTH    X64_SIZEOF_EXTENDED_REGS
#define X64_BRAND_STRING_LENGTH         (X64_BRAND_STRING_PART_LENGTH * 4)

usz x64_get_brand_string(char *buff)
{
    char brand_string_buff[X64_BRAND_STRING_LENGTH + NULL_TERM_SIZE] = {0};
    char brand_string_part_buff[X64_BRAND_STRING_PART_LENGTH + NULL_TERM_SIZE] = {0};

    if (!buff)
        return ARRAY_SIZE(brand_string_buff);

    struct str_builder brand_string_builder = {0};
    str_builder_init_ext(&brand_string_builder, M_BUFFER(brand_string_buff, ARRAY_SIZE(brand_string_buff)));

    x64_get_brand_string_part(1, brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part);

    x64_get_brand_string_part(2, brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part);

    x64_get_brand_string_part(3, brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part);

    return 0;
}
