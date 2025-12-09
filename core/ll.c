#include <core/ll.h>
#include <core/memory.h>
#include <core/utils.h>

struct ll_double_node {
    struct ll_double_node *next;
    struct ll_double_node *prev;
    void *element;
};

#define GET_ELEMENT_COUNT(ll)   (ll)->count - (ll)->available

#define NODE_TO_ELEMENT(n)   &((struct ll_double_node *)(n))[1]

static void ll_double_node_unlink(struct ll_double_node *node);
static void ll_double_node_unlink_first(struct ll_double_node *node);
static void ll_double_node_unlink_last(struct ll_double_node *node);

static void ll_double_node_insert(struct ll_double_node *node, struct ll_double_node *new);
static void ll_double_node_add_first(struct ll_double_node *node, struct ll_double_node *new);
static void ll_double_node_add_last(struct ll_double_node *node, struct ll_double_node *last);

static struct ll_double_node *get_free_node(struct ll_double *ll);
static void add_free_node(struct ll_double *ll, struct ll_double_node *node);
static void init_free_list(struct ll_double *ll);

void ll_double_init_ext(struct ll_double *ll, const struct ll_double_info info)
{
    CHECK_NULL(info.arena);

    ll->width       = info.width;
    ll->stride      = (sizeof(struct ll_double_node) + info.width);
    ll->count       = info.count;
    ll->available   = info.count;
    ll->first   = m_arena_alloc(info.arena, ll->stride, info.count);
    ll->last    = ll->first;
    ll->free    = ll->first;
    ll->heap    = TRUE;

    init_free_list(ll);
}

void ll_double_delete(struct ll_double *ll)
{
    if (ll->heap) {
        m_free(ll->first);
    }
}

void ll_double_add(struct ll_double *ll, void *element)
{
    ASSERT(ll->available, "no elements left");
    struct ll_double_node
}

void ll_double_append(struct ll_double *ll, void *element)
{
    ASSERT(ll->available, "no elements left");
    struct ll_double_node *node = get_free_node(ll);

    void *dst = (void *)&node[1];
    m_copy(dst, element, ll->width);

    node->prev = ll->last;
    node->next = NULL;

    ll->last = node;
}

void ll_double_insert(struct ll_double *ll, usz index, void *element)
{
    struct ll_double_node *iterator = ll->first;
    usz element_count = GET_ELEMENT_COUNT(ll);

    for (usz i = 0; i < element_count; ++i) {
        iterator = iterator->next;
    }

    ll_double_node_insert(iterator, element);
}

void ll_double_unlink(struct ll_double *ll, usz index)
{
    struct ll_double_node *iterator = ll->first;
    usz element_count = GET_ELEMENT_COUNT(ll)
    for (usz i = 0; i < element_count; ++i) {
        iterator = iterator->next;
    }

    add_free_node(ll, iterator);
}

void *ll_double_get_addr(struct ll_double *ll, usz index)
{
    struct ll_double_node iterator = ll->first;
    for (usz i = 0; i < GET_ELEMENT_COUNT(ll); ++i)
}

void ll_double_get(struct ll_double *ll, usz index, void *dst)
{
    m_copy(dst, ll_double_get_addr(ll, index), ll->width);
}

static void init_free_list(struct ll_double *ll)
{
    struct m_array free_array_list  = M_ARRAY_INIT(ll->free, ll->stride, ll->count);
    struct ll_double_node *node     = m_array_get_addr(&free_array_list, 0);
    node->prev  = NULL;
    node->next  = m_arrray_get_addr(&free_array_list, 1);
    node        = node->next;

    for (usz i = 1; i < ll->count - 1; ++i) {
        node->prev      = m_array_get_addr(&free_array_list, i);
        node->next      = m_array_get_addr(&free_array_list, i + 1);
        node->element   = NODE_TO_ELEMENT(node);
        node            = node->next;
    }

    node->prev = m_array_get_addr(&free_array_list, ll->count - 2);
    node->next = NULL;
}

static void ll_double_node_unlink(struct ll_double_node *node)
{
    ASSERT(node->next && node->prev, "node missing neighbour!");

    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void ll_double_node_unlink_first(struct ll_double_node *node)
{
    ASSERT(node->next, "node missing neighbour!");
    node->next->prev = NULL;
}

static void ll_double_node_insert(struct ll_double_node *node, struct ll_double_node *new)
{
    new->next = node;
    new->prev = node->prev;
    node->prev = new;
}

static void ll_double_node_add_first(struct ll_double_node *node, struct ll_double_node *first)
{
    node->prev = first;
    first->prev = NULL;
    first->next = node;
}

static void ll_double_node_add_last(struct ll_double_node *node, struct ll_double_node *last)
{
    node->prev = last;
    last->next = NULL;
    last->prev = node;
}

static struct ll_double_node *get_free_node(struct ll_double *ll)
{
    struct ll_double_node *free_node = ll->free;
    ll_double_node_unlink_first(ll->free);
    ll->free = ll->free->next;
    --ll->available;
    return free_node;
}

static void add_free_node(struct ll_double *ll, struct ll_double_node *node)
{
    ll_double_node_add_first(ll->free, node);
    ll->free = node;
    ++ll->available;
}

static void ll_double_node_unlink_last(struct ll_double_node *node)
{
    ASSERT(node->prev, "node missing neighbour!");
    node->prev->next = NULL;
}

