#include <core/utils.h>
#include <core/sparse_set.h>


Sparse_Set_Status sparse_index_set_init(struct sparse_index_set *ss,
                                        const struct sparse_set_info info)
{
    CHECK_NULL(info.sparse_buffer);
    CHECK_NULL(info.dense_buffer);
    ASSERT(info.cap, "capacity needs to be greater than 0");

    ss->sparse  = info.sparse_buffer;
    ss->dense   = info.dense_buffer;
    ss->max_val = info.max_val;
    ss->cap     = info.cap;
    ss->n       = 0;

    return 0;
}

Sparse_Set_Status sparse_index_set_swap(struct sparse_index_set *ss, usz i, usz j)
{
    SWAP(ss->dense[j], ss->dense[i]);

    return 0;
}
