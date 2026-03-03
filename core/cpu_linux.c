
#include <core/cpu.h>
#include <core/ia64.h>
#include <core/strings.h>
#include <core/log.h>

/* static function declaration start */
static char *cpu_get_name(struct m_arena *arena);
static usz cpu_get_logical_core_count(void);
/* static function declaration end */

enum vendor_id_values {
    VENDOR_ID_AMD,
    VENDOR_ID_INTEL,
    VENDOR_ID_UNKNOWN,
};

void cpu_info_query_all(struct cpu_info *cpu, struct m_arena *arena)
{
    if (arena)
        cpu->cpu_name = cpu_get_name(arena);

    cpu->logical_core_count = cpu_get_logical_core_count();
}

static char *cpu_get_name(struct m_arena *arena)
{
    char *cpu_name = NULL;

    usz string_buffer_length = x64_get_brand_string(NULL);
    cpu_name = m_arena_alloc(arena, 1, string_buffer_length);
    x64_get_brand_string(cpu_name);

    return cpu_name;
}

static usz cpu_get_logical_core_count(void)
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

