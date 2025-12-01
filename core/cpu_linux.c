#include <core/cpu.h>
#include <core/ia64.h>
#include <core/strings.h>
#include <core/log.h>

#define CPU_DEFAULT_LOGICAL_COUNT 4

#define INTEL_VENDOR_STRING "GenuineIntel"
#define AMD_VENDOR_STRING   "AuthenticAMD"

static void cpu_get_vendor_id(char **string);
static usz cpu_get_logical_count_intel(void);
static usz cpu_get_logical_count_amd(void);

usz cpu_get_logical_count(void)
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

    /* we get here if we're not on Intel AMD or for some reason can't execute CPUID */
    ERROR_LOG("could not query for logical processor count... continuing with default value");

    return CPU_DEFAULT_LOGICAL_COUNT;
}

static void cpu_get_vendor_id(char **string)
{
    static char vendor_string_buffer[sizeof(u32) * 4 + NULL_TERM_SIZE] = {0};
    struct x64_gpr gpr = {
        .rax = CPUID_FUNC_VENDOR_ID,
    };
    x64_cpuid(&gpr);

    vendor_string_buffer[0] = (char) (gpr.rax);
    vendor_string_buffer[1] = (char) (gpr.rax >> 8);
    vendor_string_buffer[2] = (char) (gpr.rax >> 16);
    vendor_string_buffer[3] = (char) (gpr.rax >> 24);

    vendor_string_buffer[4] = (char) (gpr.rbx);
    vendor_string_buffer[5] = (char) (gpr.rbx >> 8);
    vendor_string_buffer[6] = (char) (gpr.rbx >> 16);
    vendor_string_buffer[7] = (char) (gpr.rbx >> 24);

    vendor_string_buffer[8] = (char) (gpr.rcx);
    vendor_string_buffer[9] = (char) (gpr.rcx >> 8);
    vendor_string_buffer[10] = (char) (gpr.rcx >> 16);
    vendor_string_buffer[11] = (char) (gpr.rcx >> 24);

    vendor_string_buffer[12] = (char) (gpr.rdx);
    vendor_string_buffer[13] = (char) (gpr.rdx >> 8);
    vendor_string_buffer[14] = (char) (gpr.rdx >> 16);
    vendor_string_buffer[15] = (char) (gpr.rdx >> 24);

    vendor_string_buffer[16] = '\0';

    *string = vendor_string_buffer;
}

static usz cpu_get_logical_count_intel(void)
{
    return CPU_DEFAULT_LOGICAL_COUNT;
}

static usz cpu_get_logical_count_amd(void)
{
    return CPU_DEFAULT_LOGICAL_COUNT;
}


