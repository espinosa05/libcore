#include <core/utils.h>
#include <core/sparse_set.h>


Sparse_Set_Status sparse_index_set_init(struct sparse_index_set *ss,
                                        const struct sparse_set_info info)
{
    ASSERT(info.n_elements, "a sparse set requires at least 1 element");
    ASSERT(info.n_indeces, "a sparse set requires at least 1 index");

    ss->sparse          = info.sparse_buffer;
    ss->dense           = info.dense_buffer;
    ss->n_indeces       = info.n_indeces;
    ss->n_elements      = info.n_elements;
    ss->used_elements   = 0;


    return SPARSE_SET_STATUS_SUCCESS;
}

Sparse_Set_Status sparse_index_set_swap(struct sparse_index_set *ss, usz i, usz j)
{
    SWAP(ss->dense[j], ss->dense[i]);

    return SPARSE_SET_STATUS_SUCCESS;;
}

u64 sparse_index_set_get(const struct sparse_index_set *ss, u64 index)
{
    ASSERT(index <= ss->n_indeces, "index out of range!");
    return ss->dense[ss->sparse[index]];
}

