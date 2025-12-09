#ifndef __CORE_LL_H__
#define __CORE_LL_H__

#include <core/types.h>
#include <core/memory.h>

struct ll_double_info {
    usz width;
    usz count;
    struct m_arena *arena;
};

struct ll_double_node

struct ll_double {
    struct ll_double_node *first;
    struct ll_double_node *last;
    struct ll_double_node *free;
    usz stride;
    usz width;
    usz count;
    usz available;
    b32 heap;
};

void ll_double_init_ext(struct ll_double *ll, const struct ll_double_info);
void ll_double_append(struct ll_double *ll, void *element);
void ll_double_get(struct ll_double *ll, usz index, void *dst);
void ll_double_insert(struct ll_double *ll, usz index, void *element);
void ll_double_unlink(struct ll_double *ll, usz index);

#endif /* __CORE_LL_H__ */
