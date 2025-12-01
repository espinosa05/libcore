#include <core/memory.h>

int main(void)
{
    usz size = 32;
    struct m_array array = {0};
    struct m_array_info array_info = {
        .width  = sizeof(usz),
        .base   = m_alloc(1, size),
        .cap    = size,
    };

    m_array_init_ext(&array, array_info);
    m_array_append(&array, &(usz) {5});
    m_array_append(&array, &(usz) {7});
    m_array_append(&array, &(usz) {6});
    m_array_append(&array, &(usz) {3});
    m_array_append(&array, &(usz) {117});

    for (usz i = 0; i < array.count; ++i) {
        usz e = 0;
        m_array_get(&array, i, (void *)&e);
        printf("array[%d] : %d\n", i, e);
    }
    return 0;
}
