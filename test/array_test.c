#include <core/memory.h>

int main(void)
{
    usz size = 32;
    struct m_array array = {0};
    m_array_init(&array, sizeof(usz), size);

    m_array_append(&array, USZ_REF(7));
    m_array_append(&array, USZ_REF(7));
    m_array_append(&array, USZ_REF(6));
    m_array_append(&array, USZ_REF(32));
    m_array_append(&array, USZ_REF(112));

    for (usz i = 0; i < array.count; ++i) {
        usz e = *USZ_ARRAY_ELEMENT(&array, i);
        printf("array["USZ_FMT"] : "USZ_FMT"\n", i, e);
    }

    return OS_EXIT_SUCCESS;
}
