#ifndef __CORE_CPU_H__
#define __CORE_CPU_H__

#include <core/types.h>
#include <core/memory.h>

struct cpu_info {
    usz logical_core_count;
    usz physical_core_count;
    const char *cpu_name;

    struct m_array cache_sizes;
};

void cpu_info_query_all(struct cpu_info *cpu, struct m_arena *arena);

#endif /* __CORE_CPU_H__ */
