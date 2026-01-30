#include <core/memory.h>
#include <core/ia64.h>
#include <core/asm.h>
#include <core/utils.h>

/* static function declaration start */
static char *x64_get_brand_string_part(usz idx, struct m_buffer *buff);
/* static function declaration end */

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

#define X64_BRAND_STRING_PART_LENGTH    (X64_GPR_EXTENDED_REG_SIZE*4)
#define X64_BRAND_STRING_LENGTH         (X64_BRAND_STRING_PART_LENGTH * 4)

usz x64_get_brand_string(char *buff)
{
    char brand_string_char_buff[X64_BRAND_STRING_LENGTH + NULL_TERM_SIZE] = {0};
    char brand_string_part_char_buff[X64_BRAND_STRING_PART_LENGTH + NULL_TERM_SIZE] = {0};
    struct m_buffer brand_string_buff = {0};
    struct m_buffer_info brand_string_buff_info = {
        .buffer = brand_string_part_char_buff,
        .size   = ARRAY_SIZE(brand_string_part_char_buff),
    };
    m_buffer_init(&brand_string_buff, brand_string_buff_info);

    if (!buff)
        return ARRAY_SIZE(brand_string_char_buff);

    struct str_builder brand_string_builder = {0};
    str_builder_init_ext(&brand_string_builder, brand_string_buff);

    for (usz i = 0; i < 3; ++i) {
        str_builder_append(&brand_string_builder, x64_get_brand_string_part(i, &brand_string_buff));
    }

    m_copy(brand_string_char_buff, buff, ARRAY_SIZE(brand_string_char_buff));

    return 0;
}

static char *x64_get_brand_string_part(usz idx, struct m_buffer *buff)
{
    CHECK_NULL(buff);
    ASSERT_RT(idx <= 3, "can't get more than 3 cpu brand string parts");

    struct x64_gpr cpuid_regs = {0};
    cpuid_regs.rax = CPUID_FUNC_BRAND_STRING_PART_1 + idx;

    m_buffer_set_cursor(buff, 0);
    x64_cpuid(&cpuid_regs);

    m_buffer_write(buff, &cpuid_regs.rax, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(buff, &cpuid_regs.rbx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(buff, &cpuid_regs.rcx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(buff, &cpuid_regs.rdx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(buff, &(char) {'\0'}, 1, 1); /* zero terminate */

    return (char *)buff->base;
}

