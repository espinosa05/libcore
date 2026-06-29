#ifndef __CORE_MEMORY_H__
#define __CORE_MEMORY_H__

#include <core/utils.h>
#include <core/types.h>
#include <core/cstd.h> /* memset, malloc, realloc, free */
#include <core/platform.h>
#include <core/os_lock.h>


enum mem_sizes {
    BYTE_SIZE   = 1,
    KB_SIZE     = 1 << 10,
    MB_SIZE     = 1 << 20,
    GB_SIZE     = 1 << 30,
};

#define WORD_SIZE sizeof(usz)
#define BITS_IN_BYTE 8
#ifdef CORE_PLATFORM_COMPILER_GNUC
#define M_FENCE() asm volatile ("" ::: "memory")
#else
#define M_FENCE() __EMPTY_MACRO__
#endif /* CORE_PLATFORM_COMPILER_GNUC */

#define PAGE_SIZE (KB_SIZE*4)
#define PAGES(c) (PAGE_SIZE*c)

#define m_alloc(s, c)       malloc(s*c)
#define m_realloc(p, s, c)  realloc(p, s*c)

#define m_free  free
#define m_set   memset
#define m_copy  memcpy
#define m_move  memmove

#define m_zero(p, s)    \
    MACRO_START         \
        M_FENCE();      \
        m_set(p, 0, s); \
    MACRO_END

struct m_array {
    void    *data;
    usz     width;
    usz     count;
    usz     cap;
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
void m_array_get(const struct m_array *array, usz index, void *element);
void *m_array_get_addr(const struct m_array *array, usz index);
void m_array_append(struct m_array *array, void *element);
void m_array_copy(const struct m_array *src, struct m_array *dst);
void m_array_delete(struct m_array array);

#define M_ARRAY_FMT "{" STR_NL                      \
                        STR_TAB PTR_FMT "," STR_NL  \
                        STR_TAB USZ_FMT "," STR_NL  \
                        STR_TAB USZ_FMT "," STR_NL  \
                    "}"

#define M_ARRAY_FMT_ARG(a)  (a).data, (a).count, (a).width

struct m_stack {
    void *base;
    usz element_size;
    usz sp;
    usz cap;
};
#define M_STACK(...) (struct m_stack) { __VA_ARGS__ }
#define M_STACK_REF(...) &M_STACK(__VA_ARGS__)

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

#define M_STACK_CALL(c)                                                 \
    MACRO_START                                                         \
        M_Stack_Status st = (c);                                        \
        if (st >= M_STACK_SUCCESSFUL_STATUS_COUNT) {                    \
            THROW_EXCEPTION("call to '"STR_SYM(c)"' failed : STR_FMT",  \
                            m_stack_get_status_str(st));                \
        }                                                               \
    MACRO_END

M_Stack_Status m_stack_init(struct m_stack *stack, const struct m_stack_info info);
M_Stack_Status m_stack_push(struct m_stack *stack, const void *element);
M_Stack_Status m_stack_pop(struct m_stack *stack, void *element);
M_Stack_Status m_stack_push_array(struct m_stack *stack, const struct m_array array);
M_Stack_Status m_stack_delete(const struct m_stack stack);
const char *m_stack_get_status_str(usz st);

#endif /* __CORE_MEMORY_H__ */
