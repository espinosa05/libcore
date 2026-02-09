#include <stdio.h>


#define ll_double(type, cap)            \
    struct ll_double_ ## __COUNTER__ {  \
        type *start;                    \
        type *end;                      \
        type *free_list;                \
        type data[cap];                 \
    }

#define ll_double_append(list, element) \
    do {
        *(list)->end = element;
        (list)->end = 
    } while (0)

#define ll_double_init(list)                    \
    do {                                        \
        (list)->start = (list)->data;           \
        (list)->end = (list)->start;            \
        (list)->free_list = (list)->data;       \
    } while (0)


int main(void)
{
    ll_double(uint32_t, 16) numbers = {0};

    ll_double_init(&numbers);
    ll_double(uint32_t, 8) results = {0};

    for (EACH_LL_DOUBLE(list, entry)) {
        printf(, entry->element);
    }

    return 0;
}
