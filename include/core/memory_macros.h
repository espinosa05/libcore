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

#define MM_DBG_ARRAY_INDEX_CHECK(mm_a, i) ASSERT(i < (mm_a)->cap, "MM_ARRAY("USZ_FMT") INDEX ["USZ_FMT"] OUT OF BOUNDS!", (mm_a)->cap, (i))

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

#define mm_array_init(mm_a, init_cap)                                   \
    MACRO_START                                                         \
        (mm_a)->cap     = init_cap;                                     \
        (mm_a)->count   = 0;                                            \
        (mm_a)->data    = m_alloc(sizeof(*(mm_a)->data), (mm_a)->cap);  \
    MACRO_END

#define mm_array_init_ext(mm_a, buff, buff_cap) \
    MACRO_START                                 \
        (mm_a)->cap = buff_cap;                 \
        (mm_a)->count = 0;                      \
        (mm_a)->data = buff;                    \
    MACRO_END

#define mm_array_init_ar(mm_a, arr_cap, ar)                                 \
    MACRO_START                                                             \
        (mm_a)->cap = arr_cap;                                              \
        (mm_a)->count = 0;                                                  \
        (mm_a)->data = m_arena_alloc((ar), sizeof(*(mm_a)->data), arr_cap); \
    MACRO_END

#define mm_array_free_space(mm_a)   ((mm_a)->cap - (mm_a)->count)
#define mm_array_index_oob(mm_a, i) ((mm_a)->cap > i)

#define mm_array_append(mm_a, e)    (mm_a)->data[(mm_a)->count++] = (e)
#define mm_array_insert(mm_a, i, e) m_move(&(mm_a)->data[i + 1], &(mm_a)->data[i], sizeof(*(mm_a)->data) * mm_array_get_free_space(mm_a) - 1)
#define mm_array_remove(mm_a, i)    m_move(&(mm_a)->data[i], &(mm_a)->data[i + 1], sizeof(*(mm_a)->data) * mm_array_get_free_space(mm_a) + 1)

#define mm_array_swap(mm_a, i0, i1)                 \
    MACRO_START                                     \
        MM_DBG_ARRAY_INDEX_CHECK(mm_a, i0);         \
        MM_DBG_ARRAY_INDEX_CHECK(mm_a, i1);         \
        SWAP((mm_a)->data[i0], (mm_a)->data[i1]);   \
    MACRO_END

#define MM_QUEUE_MEMBERS(type)  \
    usz cap;                    \
    usz l_bounds;               \
    usz u_bounds;               \
    type *data

#define mm_queue_init(mm_q, init_cap)                                       \
    MACRO_START                                                             \
        (mm_q)->cap         = init_cap;                                     \
        (mm_q)->l_bounds    = 0;                                            \
        (mm_q)->u_bounds    = 0;                                            \
        (mm_q)->data        = m_alloc(sizeof(*(mm_q)->data), (mm_q)->cap);  \
    MACRO_END

#define MM_RING_BUFF_INDEX(i, cap)  (i % cap)
#define mm_queue_length(mm_q)                               \
    DIFF(MM_RING_BUFF_INDEX((mm_q)->l_bounds, (mm_q)->cap), \
         MM_RING_BUFF_INDEX((mm_q)->u_bounds, (mm_q)->cap))

#define mm_queue_enqueue(mm_q, e)                                                       \
    MACRO_START                                                                         \
        ASSERT(MM_RING_BUFF_INDEX((mm_q)->u_bounds + 1, (mm_q)->cap)                    \
                != MM_RING_BUFF_INDEX((mm_q)->l_bounds, (mm_q)->cap),                   \
                "MM_QUEUE("USZ_FMT") FULL... CAN'T ENQUEUE!",                           \
                (mm_q)->cap);                                                           \
        if (mm_queue_length(mm_q) + 1 > (mm_q)->cap) {                                  \
            (mm_q)->cap *= 2;                                                           \
            (mm_q)->data = m_realloc((mm_q)->data, sizeof(*(mm_q)->data), (mm_q)->cap); \
        }                                                                               \
        (mm_q)->data[(mm_q)->u_bounds++] = e;                                           \
    MACRO_END

#define mm_queue_dequeue(mm_q, e)                                       \
    MACRO_START                                                         \
        ASSERT(MM_RING_BUFF_INDEX((mm_q)->l_bounds - 1, (mm_q)->cap)    \
                != MM_RING_BUFF_INDEX((mm_q)->u_bounds, (mm_q)->cap),   \
                "MM_QUEUE("USZ_FMT") EMPTY... CAN'T DEQUEUE!",          \
                (mm_q)->cap);                                           \
        (*e) = (mm_q)->data[(mm_q)->l_bounds--];                        \
    MACRO_END


#define MM_SPARSE_SET_MEMBERS(type) \
    type *sparse;                   \
    usz *dense;                     \
    usz n_indeces;                  \
    usz n_elements;                 \
    usz used;

#define mm_sparse_set_init(ss, i, e)                                \
    MACRO_START                                                     \
        ASSERT(e <= i, "not enough indeces for element buffer!");   \
        (ss)->sparse    = m_alloc(sizeof(*(ss)->sparse), e);        \
        (ss)->dense     = m_alloc(sizeof(*(ss)->dense), i);         \
    MACRO_END

#define mm_sparse_set_swap(ss, i, j)            \
    MACRO_START                                 \
        SWAP((ss)->dense[j], (ss)->dense[i]);   \
    MACRO_END

#define mm_sparse_set_get(ss, i)    (ss)->dense[(ss)->sparse[i]]

#endif /* __CORE_MEMORY_MACROS_H__ */
