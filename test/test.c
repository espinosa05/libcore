#include <core/cstd.h>
#include <core/strings.h>
#include <core/memory.h>
#include <core/os.h>
#include <core/os_file.h>
#include <core/os_socket.h>
#include <core/os_streams.h>
#include <core/os_dynamic_library.h>
#include <core/wm.h>


void test_memory_allocation(void)
{
}

void test_memory_arena(void)
{
}

#define STR_BUILDER_EXPECTED_STRING "this is a test string!"
void test_string_builder(void)
{
    char *test_result = NULL;
    struct str_builder sb = {0};
    str_builder_init(&sb, 256);

    str_builder_append(&sb, "this ");
    str_builder_append(&sb, "is ");
    str_builder_append(&sb, "a ");
    str_builder_append(&sb, "test ");
    str_builder_append(&sb, "string!");

    str_builder_to_cstr_alloc(&sb, &test_result);

    ASSERT_RT(cstr_compare(STR_BUILDER_EXPECTED_STRING, test_result),
              "test strings do not match!\n"
              "expected string:\t%s\n"
              "generated string:\t%s\n",
              STR_BUILDER_EXPECTED_STRING,
              test_result);

    m_free(test_result);
    str_builder_delete(sb);
}

void test_os_file(void)
{
}

void test_os_socket(void)
{
}

void test_os_streams(void)
{
}

int main(int argc, char **argv)
{
    test_string_builder();
    return EXIT_SUCCESS;
}
