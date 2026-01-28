#include <core/memory.h>
#include <core/ia64.h>
#include <core/asm.h>
#include <core/utils.h>

/* static function declaration start */
static void x64_get_brand_string_part(usz idx, struct m_buffer *brand_string_part);
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

    if (!buff)
        return ARRAY_SIZE(brand_string_char_buff);

    struct m_buffer brand_string_buff = {0};
    struct m_buffer_info brand_string_buff_info = {
        .buffer = &brand_string_char_buff,
        .size   = ARRAY_SIZE(brand_string_char_buff),
    };
    m_buffer_init(&brand_string_buff, brand_string_buff_info);

    struct m_buffer brand_string_part_buff = {0};
    struct m_buffer_info brand_string_part_buff_info = {
        .buffer = &brand_string_part_char_buff,
        .size   = ARRAY_SIZE(brand_string_part_char_buff),
    };
    m_buffer_init(&brand_string_part_buff, brand_string_part_buff_info);

    struct str_builder brand_string_builder = {0};
    str_builder_init_ext(&brand_string_builder, brand_string_buff);

    x64_get_brand_string_part(0, &brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part_buff.base);

    x64_get_brand_string_part(1, &brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part_buff.base);

    x64_get_brand_string_part(2, &brand_string_part_buff);
    str_builder_append(&brand_string_builder, brand_string_part_buff.base);

    return 0;
}

static void x64_get_brand_string_part(usz idx, struct m_buffer *brand_string_part)
{
    ASSERT_RT(idx <= 3, "can't get more than 3 cpu brand string parts");
    ASSERT_RT(brand_string_part->size <= X64_BRAND_STRING_PART_LENGTH + NULL_TERM_SIZE,
              STR_SYM_QUOT(brand_string_part)" needs to be "USZ_FMT" bytes big. Is "USZ_FMT" bytes big",
              X64_BRAND_STRING_PART_LENGTH + NULL_TERM_SIZE, brand_string_part->size);

    struct x64_gpr cpuid_regs = {0};
    cpuid_regs.rax = CPUID_FUNC_BRAND_STRING_PART_1 + idx;
    m_buffer_set_cursor(brand_string_part, 0);
    x64_cpuid(&cpuid_regs);

    m_buffer_write(brand_string_part, &cpuid_regs.rax, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(brand_string_part, &cpuid_regs.rbx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(brand_string_part, &cpuid_regs.rcx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(brand_string_part, &cpuid_regs.rdx, X64_GPR_EXTENDED_REG_SIZE, X64_GPR_EXTENDED_REG_SIZE);
    m_buffer_write(brand_string_part, &(char) {'\0'}, 1, 1); /* zero terminate */
}

