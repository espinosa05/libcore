#include <core/cpu.h>
#include <core/ia64.h>
#include <core/strings.h>
#include <core/log.h>

#define CPU_DEFAULT_LOGICAL_COUNT 4

#define INTEL_VENDOR_STRING "GenuineIntel"
#define AMD_VENDOR_STRING   "AuthenticAMD"

static void cpu_get_vendor_id(char **string);

static void cpu_get_name();
static void cpu_get_physical_core_count(struct cpu_info *info);
static usz cpu_get_logical_core_count_intel(void);
static usz cpu_get_logical_core_count_amd(void);

void cpu_info_query_all(struct cpu_info *cpu, struct m_arena *arena)
{
    cpu_get_name(cpu, arena);
    cpu_get_logical_core_count(cpu);
    cpu_get_physical_core_count(cpu);
    cpu_get_cache_sizes(cpu, arena);
}

static usz cpu_get_physical_count(void)
{
    FIXME("query physical core count");
    return cpu_get_logical_count();
}

static usz cpu_get_logical_count(void)
{
    if (x64_has_cpuid()) {
        char *vendor_id = NULL;
        cpu_get_vendor_id(&vendor_id);

        /* AMD and Intel handle things differently for some reason */
        if (cstr_compare(INTEL_VENDOR_STRING, vendor_id))
            return cpu_get_logical_count_intel();

        if (cstr_compare(AMD_VENDOR_STRING, vendor_id))
            return cpu_get_logical_count_amd();
    }

    /* we get here if we're not on Intel, AMD or for some reason can't execute CPUID */
    ERROR_LOG("could not query for logical processor count... continuing with default value");

    return CPU_DEFAULT_LOGICAL_COUNT;
}

#define CPUID_REG_COUNT 4
#define CPUID_VENDOR_STRING_LENGTH (sizeof(u32) * CPUID_REG_COUNT)

static void cpu_get_vendor_id(char **string)
{
    struct x64_gpr gpr = {0};
    static char vendor_string_buffer[CPUID_VENDOR_STRING_LENGTH + NULL_TERM_SIZE] = {0};

    X64_GPR_SET_EAX(&gpr, CPUID_FUNC_VENDOR_ID)
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

static char *cpu_get_name(struct m_arena *arena)
{
    x64_get_brand_string();
}

static usz cpu_get_logical_count_intel(void)
{
    FIXME("query intel cpu cores");
    return CPU_DEFAULT_LOGICAL_COUNT;
}

static usz cpu_get_logical_count_amd(void)
{
    FIXME("query amd cpu cores");
    return CPU_DEFAULT_LOGICAL_COUNT;
}


