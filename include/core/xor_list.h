#ifndef __CORE_XOR_LIST_H__
#define __CORE_XOR_LIST_H__

#include <core/types.h>
#include <core/memory.h>

struct xor_node {
    struct xor_node *link;
    void *element;
};

struct xor_list_info {
    struct m_arena *arena;
    usz width;
    usz cap;
};

struct xor_list {
    usz width;
    struct xor_node *first;
    struct xor_node *last;
};

void xor_list_init(struct xor_list *xl, const struct xor_list_info info);
void xor_list_add(struct xor_list *xl, void *element);
void xor_list_insert(struct xor_list *xl, void *element);
void xor_list_remove(struct xor_list *xl, usz index);

#endif /* __CORE_XOR_LIST_H__ */
