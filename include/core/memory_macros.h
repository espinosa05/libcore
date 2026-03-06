#ifndef __CORE_MEMORY_MACROS_H__
#define __CORE_MEMORY_MACROS_H__

#include <core/types.h>
#include <core/macros.h>
#include <core/memory.h>
#include <core/utils.h>

/*
 * This api should be used as a template for array and stack data structures.
 *
 * whenever you create a structure that should behave like an array, do the following
 *
 * ```c
 * struct my_struct {
 *     some_other_type_t metadata;
 *     ...
 *     MM_ARRAY_MEMBERS(some_type_t);
 * }
 * ```
 *
 * this will add 3 member variables to your struct that you won't have to interact with.
 * They are used to track the current state of the array.
 *
 * to initialize your array, do:
 * ```c
 * struct my_struct s = {0};
 * mm_array_init(&s, CAPACITY);
 * ```
 *
 * because MM_ARRAY_MEMBERS is a partial set of MM_STACK_MEMBERS
 * mm_* macros using the cap and data fields are compatible with eachother
 *
 * */

#define MM_STACK_MEMBERS_STATIC(type, cap)  \
    usz pointer;                            \
    type data[cap]

#define MM_ARRAY_MEMBERS_STATIC(type, cap)  \
    usz count;                              \
    type data[cap]

#define MM_STACK_MEMBERS(type)  \
    usz cap;                    \
    usz pointer;                \
    type *data

#define mm_stack_push(mm_s, e)  (mm_s)->data[(mm_s)->pointer++] = (e)
#define mm_stack_pop(mm_s)      (mm_s)->data[(mm_s)->pointer == 0 ? (mm_s)->pointer : --(mm_s)->pointer]

#define MM_ARRAY_MEMBERS(type)  \
    usz cap;                    \
    usz count;                  \
    type *data

#define mm_array_set(mm_a, i, e)    (mm_a)->data[i] = (e)
#define mm_array_get(mm_a, i)       (mm_a)->data[i]

#define mm_array_free_space(mm_a)   ((mm_a)->cap - (mm_a)->count)
#define mm_array_index_oob(mm_a, i) ((mm_a)->cap > i)

#define mm_array_append(mm_a, e)    (mm_a)->data[(mm_a)->count++] = (e)
#define mm_array_insert(mm_a, i, e) m_move(&(mm_a)->data[i + 1], &(mm_a)->data[i], sizeof(*(mm_a)->data) * mm_array_get_free_space(mm_a) - 1)
#define mm_array_remove(mm_a, i)    m_move(&(mm_a)->data[i], &(mm_a)->data[i + 1], sizeof(*(mm_a)->data) * mm_array_get_free_space(mm_a) + 1)



#endif /* __CORE_MEMORY_MACROS_H__ */
