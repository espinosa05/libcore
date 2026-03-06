#include <core/utils.h>
#include <core/macros.h>
#include <core/memory.h>
#define SIZE 32
int main(void)
{
    struct m_dynamic_array(usz) test_array_1 = {0};
    struct m_static_array(usz, SIZE) test_array_2 = {0};

    m_dynamic_array_init(&test_array_1, 32);

    for (usz i = 0; i < ARRAY_SIZE(test_array_2.data); ++i) {
        m_dynamic_array_push(&test_array_1, i);
        m_static_array_push(&test_array_2, i);
    }

    for (usz i = 0; i < ARRAY_SIZE(test_array_2.data); ++i) {
        LOG(STR_SYM(struct m_dynamic_array) "["USZ_FMT"]" STR_TAB "= "  USZ_FMT STR_NL, i, m_dynamic_array_get(&test_array_1,    i));
        LOG(STR_SYM(struct m_static_array)  "["USZ_FMT"]" STR_TAB "= "  USZ_FMT STR_NL, i, m_static_array_get(&test_array_2,     i));
    }

    return EXIT_SUCCESS;
}
