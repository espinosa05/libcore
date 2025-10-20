#include <core/native_ui.h>
#include <core/utils.h>

void assert_gui_impl(char *assert_string, usz line, char *file, char *func,
                    char *msg, char *detailed_error_fmt, ...)
{
    UNUSED(assert_string);
    UNUSED(line);
    UNUSED(file);
    UNUSED(func);
    UNUSED(msg);
    UNUSED(detailed_error_fmt);
    IMPL();
}

