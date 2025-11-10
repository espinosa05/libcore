#ifndef __CORE_SPARSE_SET_H__
#define __CORE_SPARSE_SET_H__

#include <core/types.h>

struct sparse_index_set {
    u64 *sparse;
    u64 *dense;
    usz max_val;
    usz cap;
    usz n;
};

struct sparse_set_info {
    usz max_val;
    usz cap;
    void *dense_buffer;
    void *sparse_buffer;
};

typedef usz Sparse_Set_Status;

Sparse_Set_Status sparse_index_set_init(struct sparse_index_set *ss, const struct sparse_set_info info);

#endif /* __CORE_SPARSE_SET_H__ */
