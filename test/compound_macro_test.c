#include <core/types.h>
#include <core/strings.h>
#include <core/os.h>

struct data {
    usz d0;
    usz d1;
    char *s0;
};

#define COMP_LIT(type, ...) (type) {__VA_ARGS__}

struct data get_data(void)
{
    return COMP_LIT(struct data, 3, 63, "hello");
}

int main(int argc, char **argv)
{
    struct data test_data = {0};
    test_data = get_data();

    printf(USZ_FMT STR_NL
           USZ_FMT STR_NL
           STR_FMT STR_NL,
           test_data.d0, test_data.d1, test_data.s0);

    return OS_EXIT_SUCCESS;
}
