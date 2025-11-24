#ifndef __CORE_SPARSE_SET_H__
#define __CORE_SPARSE_SET_H__

#include <core/types.h>
#include<core/memory.h>

struct sparse_index_set {
    u64 *sparse;
    u64 *dense;

    usz n_indeces;
    usz n_elements;
    usz used_elements;
};

struct sparse_set_info {
    usz n_indeces;
    usz n_elements;

    void *sparse_buffer;
    void *dense_buffer;
};
enum sparse_set_status_codes {
    SPARSE_SET_STATUS_SUCCESS = 0,

    SPARSE_SET_STATUS_COUNT,
};
typedef usz Sparse_Set_Status;

Sparse_Set_Status sparse_index_set_init(struct sparse_index_set *ss, const struct sparse_set_info info);
Sparse_Set_Status sparse_index_set_swap(struct sparse_index_set *ss, usz i, usz j);
u64 sparse_index_set_get(const struct sparse_index_set *ss, u64 index);

#endif /* __CORE_SPARSE_SET_H__ */
