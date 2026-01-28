
#include <core/cpu.h>
#include <core/ia64.h>
#include <core/strings.h>
#include <core/log.h>

#define CPU_DEFAULT_CORE_COUNT 4
#define CPU_DEFAULT_LOGICAL_COUNT 1

#define INTEL_VENDOR_STRING "GenuineIntel"
#define AMD_VENDOR_STRING   "AuthenticAMD"

/* static function declaration start */
#if COMMENT
static void cpu_get_vendor_id(char **string);
#endif /* COMMENT */

static void cpu_get_name(struct cpu_info *cpu, struct m_arena *arena);
static void cpu_get_cache_sizes(struct cpu_info *info, struct m_arena *arena);
/* static function declaration end */

void cpu_info_query_all(struct cpu_info *cpu, struct m_arena *arena)
{
    cpu_get_name(cpu, arena);
    FIXME("implement actual core count queries... i won't bother w/ ts rn");
    cpu->logical_core_count = CPU_DEFAULT_CORE_COUNT;
    cpu->physical_core_count = CPU_DEFAULT_CORE_COUNT;

    cpu_get_cache_sizes(cpu, arena);
}

#define CPUID_REG_COUNT 4
#define CPUID_VENDOR_STRING_LENGTH (sizeof(u32) * CPUID_REG_COUNT)

#if COMMENT
static void cpu_get_vendor_id(char **string)
{
    struct x64_gpr gpr = {CPUID_FUNC_VENDOR_ID};
    static char vendor_string_buffer[CPUID_VENDOR_STRING_LENGTH + NULL_TERM_SIZE] = {0};

    x64_cpuid(&gpr);
    {
        u32 eax = X64_GPR_EAX(gpr);
        vendor_string_buffer[0]     = (char) 0xFF & (eax);
        vendor_string_buffer[1]     = (char) 0xFF & (eax >> 8);
        vendor_string_buffer[2]     = (char) 0xFF & (eax >> 16);
        vendor_string_buffer[3]     = (char) 0xFF & (eax >> 24);
    }

    {
        u32 ebx = X64_GPR_EBX(gpr);
        vendor_string_buffer[4]     = (char) 0xFF & (ebx);
        vendor_string_buffer[5]     = (char) 0xFF & (ebx >> 8);
        vendor_string_buffer[6]     = (char) 0xFF & (ebx >> 16);
        vendor_string_buffer[7]     = (char) 0xFF & (ebx >> 24);
    }

    {
        u32 ecx = X64_GPR_ECX(gpr);
        vendor_string_buffer[8]     = (char) 0xFF & (ecx);
        vendor_string_buffer[9]     = (char) 0xFF & (ecx >> 8);
        vendor_string_buffer[10]    = (char) 0xFF & (ecx >> 16);
        vendor_string_buffer[11]    = (char) 0xFF & (ecx >> 24);
    }

    {
        u32 edx = X64_GPR_EDX(gpr);
        vendor_string_buffer[12]    = (char) 0xFF & (edx);
        vendor_string_buffer[13]    = (char) 0xFF & (edx >> 8);
        vendor_string_buffer[14]    = (char) 0xFF & (edx >> 16);
        vendor_string_buffer[15]    = (char) 0xFF & (edx >> 24);
    }

    NULL_TERM_BUFF(vendor_string_buffer, CPUID_VENDOR_STRING_LENGTH);
    *string = vendor_string_buffer;
}
#endif /* COMMENT */

static void cpu_get_name(struct cpu_info *cpu, struct m_arena *arena)
{
    FIXME("brand string");
    cpu->cpu_name = "PLACE_HOLDER";
    return;

    usz string_length = x64_get_brand_string(NULL);
    cpu->cpu_name = m_arena_alloc(arena, 1, string_length + NULL_TERM_SIZE);
    x64_get_brand_string((char *)cpu->cpu_name);
}

static void cpu_get_cache_sizes(struct cpu_info *info, struct m_arena *arena)
{
    UNUSED(info);
    UNUSED(arena);
}
