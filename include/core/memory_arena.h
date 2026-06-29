#ifndef __CORE_MEMORY_ARENA_H__
#define __CORE_MEMORY_ARENA_H__

#include <core/types.h>

struct m_arena {
    void *buffer;
    usz mem_avail;
    usz mem_used;
    b32 heap;
};
#define M_ARENA(...) (struct m_arena) { __VA_ARGS__ }
#define M_ARENA_REF(...) &M_ARENA(__VA_ARGS__)

#define M_ARENA_FMT         "{ .buffer="PTR_FMT", .mem_avail="USZ_FMT", .mem_used="USZ_FMT", .heap="B32_FMT", }"
#define M_ARENA_FMT_ARG(ar) (ar).buffer, (ar).mem_avail, (ar).mem_used, B32_FMT_ARG((ar).heap)

struct m_arena_info {
    usz mem_size;
    void *buffer;
    b32 external;
};

/* profile */
#define m_arena_init(ar, inf)                                               \
    MACRO_START                                                             \
        INFO_LOG("Creating new Arena of "USZ_FMT" bytes", inf.mem_size);    \
        _m_arena_init(ar, inf);                                             \
    MACRO_END

#define m_arena_alloc(ar, size, count)                                          \
    ({                                                                          \
        INFO_LOG("Allocating "USZ_FMT"*"USZ_FMT" bytes ", size, count);         \
        void *buff = _m_arena_alloc((ar), size, count);                         \
        INFO_LOG(USZ_FMT" bytes available", (ar)->mem_avail-(ar)->mem_used);    \
        buff;                                                                   \
    })

#define m_arena_destroy(ar)                                                 \
    MACRO_START                                                             \
        INFO_LOG("Destroying Arena of "USZ_FMT" bytes", (ar)->mem_avail);   \
        _m_arena_destroy(ar);                                               \
    MACRO_END

#define m_arena_save(ar, save)                                      \
    MACRO_START                                                     \
        _m_arena_save(ar, save);                                    \
        INFO_LOG("Saving Arena at offset "USZ_FMT, (ar)->mem_used); \
    MACRO_END

#define m_arena_restore(ar, save)                               \
    MACRO_START                                                 \
        _m_arena_restore(ar, save);                             \
        INFO_LOG("Restoring Arena at offset "USZ_FMT, *save);   \
    MACRO_END

#define m_arena_clear(ar)                                               \
    MACRO_START                                                         \
        INFO_LOG("Cleared Arena of "USZ_FMT" bytes", (ar)->mem_avail);  \
        INFO_LOG("Cleared "USZ_FMT" bytes in use", (ar)->mem_used);     \
        _m_arena_clear(ar);                                             \
    MACRO_END

void _m_arena_init(struct m_arena *arena, const struct m_arena_info info);
void *_m_arena_alloc(struct m_arena *arena, usz size, usz count);
void _m_arena_destroy(struct m_arena *arena);

void _m_arena_save(struct m_arena *arena, usz *save);
void _m_arena_restore(struct m_arena *arena, usz save);

void _m_arena_clear(struct m_arena *arena);

#endif /* __CORE_MEMORY_ARENA_H__ */
