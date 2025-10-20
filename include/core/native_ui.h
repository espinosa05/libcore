#ifndef __CORE_NATIVE_UI_H__
#define __CORE_NATIVE_UI_H__

#include <core/error_report.h>

#define GUI_ASSERT(assertion, ...)                                              \
    if (!(assertion)) {                                                         \
        assert_gui_impl(#assertion, __LINE__, __FILE__, __func__, __VA_ARGS__); \
    }

void assert_gui_impl(char *assert_string, usz line, char *file, char *func,
                    char *msg, char *detailed_error_fmt, ...);

#endif /* __CORE_NATIVE_UI_H__ */
