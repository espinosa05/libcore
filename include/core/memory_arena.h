#ifndef __CORE_MEMORY_ARENA_H__
#define __CORE_MEMORY_ARENA_H__

#include <core/os_lock.h>
#include <core/types.h>

struct m_arena {
    void *buffer;
    usz mem_avail;
    usz mem_used;
    b32 heap;
    struct os_mutex access;
};
#define M_ARENA(...) (struct m_arena) { __VA_ARGS__ }
#define M_ARENA_REF(...) &M_ARENA(__VA_ARGS__)

struct m_arena_info {
    usz mem_size;
    void *buffer;
    b32 external;
};

void m_arena_init(struct m_arena *arena, const struct m_arena_info info);
void *m_arena_alloc(struct m_arena *arena, usz size, usz count);
void m_arena_destroy(struct m_arena *arena);

#define m_arena_claim(ap)   os_mutex_lock(&(ap)->access)
#define m_arena_release(ap) os_mutex_unlock(&(ap)->access)
void *m_arena_alloc_claimed(struct m_arena *arena, usz size, usz count);

void m_arena_save(struct m_arena *arena, usz *save);
void m_arena_restore(struct m_arena *arena, usz save);

void m_arena_clear(struct m_arena *arena);

#endif /* __CORE_MEMORY_ARENA_H__ */
