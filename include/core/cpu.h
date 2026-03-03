#ifndef __CORE_CPU_H__
#define __CORE_CPU_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/memory_arena.h>

struct cpu_info {
    usz logical_core_count;
    const char *cpu_name;
};

void cpu_info_query_all(struct cpu_info *cpu, struct m_arena *arena);

#endif /* __CORE_CPU_H__ */
