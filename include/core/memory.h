#ifndef __CORE_MEMORY_H__
#define __CORE_MEMORY_H__

#include <core/utils.h>
#include <core/types.h>
#include <core/error_report.h>
#include <core/cstd.h> /* memset, malloc, realloc, free */

enum mem_sizes {
    BYTE_SIZE   = 1,
    KB_SIZE     = 1 << 10,
    MB_SIZE     = 1 << 20,
    GB_SIZE     = 1 << 30,
};

#define WORD_SIZE sizeof(usz)

#ifdef GNUC
#define MEMORY_BARRIER() asm volatile ("" ::: "memory")
#else
#define MEMORY_BARRIER()
#endif

#define PAGE_SIZE (KB_SIZE*4)
#define PAGES(c) (PAGE_SIZE*c)

#define m_alloc(s, c)       malloc(s*c)
#define m_free(p)           free(p)
#define m_realloc(p, s, c)  realloc(p, s*c)
#define m_set(p, b, c)      memset(p, b, c)
#define m_copy              memcpy

#define m_zero(p, s)        \
    MACRO_START             \
        MEMORY_BARRIER();   \
        m_set(p, 0, s);     \
        MEMORY_BARRIER();   \
    MACRO_END

struct m_arena {
    void *buffer;
    usz mem_avail;
    usz mem_used;
    b32 heap;
};

struct m_arena_info {
    usz mem_size;
    void *buffer;
    b32 external;
};

void m_arena_init(struct m_arena *arena, const struct m_arena_info info);
void *m_arena_alloc(struct m_arena *arena, usz size, usz count);
void m_arena_destroy(struct m_arena *arena);

struct m_buffer {
    void *base;
    usz size;
    usz cursor;
};

struct m_buffer_info {
    void *buffer;
    usz size;
};

enum m_buffer_status_codes {
    M_BUFFER_STATUS_SUCCESS = 0,
    M_BUFFER_STATUS_OUT_OF_MEMORY,
    M_BUFFER_STATUS_OUT_OF_BOUNDS_READ,
    M_BUFFER_STATUS_OUT_OF_BOUNDS_WRITE,
};
typedef u32 M_Buffer_Status;

void m_buffer_init(struct m_buffer *buffer, const struct m_buffer_info info);
M_Buffer_Status m_buffer_read(struct m_buffer *buffer, void *dst, usz dst_cap, usz ammount);
M_Buffer_Status m_buffer_write(struct m_buffer *buffer, void *dst, usz dst_cap, usz ammount);

struct m_array {
    void    *data;
    usz     cap;
    usz     count;
    usz     width;
    b32     dynamic;
};

struct m_array_info {
    void    *base;
    usz     width;
    usz     count;
    usz     cap;
};

void m_array_init(struct m_array *array, usz width, usz init_size);
/* assign an existing buffer to an array structure */
void m_array_init_ext(struct m_array *array, const struct m_array_info info);
void m_array_insert(struct m_array *array, usz index, void *element);
void m_array_get(const struct m_array *arry, usz index, void *element);
void m_array_append(struct m_array *array, void *element);
void m_array_copy(const struct m_array *src, struct m_array *dst);
void m_array_delete(struct m_array array);

struct m_stack {
    void *base;
    usz element_size;
    usz sp;
    usz cap;
};

struct m_stack_info {
    void *buffer;
    usz element_size;
    usz cap;
    b32 external;
};

/*
 * status code distribution:
 *
 *      SUCCESSFUL_STATUS_COUNT  = |SUCCESS_ENUM|
 *      STATUS_COUNT             = |SUCCESS_ENUM| + |ERROR_ENUM|
 *      ERROR_STATUS_COUNT       = STATUS_COUNT - SUCCESSFUL_STATUS_COUNT
 * */
enum m_stack_success_status_codes {
    M_STACK_STATUS_SUCCESS = 0,
    M_STACK_STATUS_LAST_ELEMENT,
    M_STACK_STATUS_EXHAUSTED,

    M_STACK_SUCCESSFUL_STATUS_COUNT,
};
typedef usz M_Stack_Status;

enum m_stack_error_status_codes {
#define M_STACK_STATUS_
    M_STACK_STATUS_OUT_OF_MEMORY = M_STACK_SUCCESSFUL_STATUS_COUNT,

    M_STACK_STATUS_UNKNOWN,
    M_STACK_STATUS_COUNT,
#define M_STACK_ERROR_STATUS_COUNT (M_STACK_STATUS_COUNT - M_STACK_SUCCESSFUL_STATUS_COUNT)
};

#define M_STACK_CALL(c)                                             \
    MACRO_START                                                     \
        M_Stack_Status st = (c);                                    \
        if (st >= M_STACK_SUCCESSFUL_STATUS_COUNT) {                \
            THROW_EXCEPTION("call to '"STR_SYM(c)"' failed : %s",   \
                            m_stack_get_status_str(st));            \
        }                                                           \
    MACRO_END

M_Stack_Status m_stack_init(struct m_stack *stack, const struct m_stack_info info);
M_Stack_Status m_stack_push(struct m_stack *stack, const void *element);
M_Stack_Status m_stack_pop(struct m_stack *stack, void *element);
M_Stack_Status m_stack_push_array(struct m_stack *stack, const struct m_array array);
M_Stack_Status m_stack_delete(const struct m_stack stack);
const char *m_stack_get_status_str(usz st);

#endif /* __CORE_MEMORY_H__ */
