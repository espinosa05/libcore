#ifndef __CORE_WM_UTILS_H__
#define __CORE_WM_UTILS_H__

#include <core/utils.h>
#include <core/types.h>

#define WM_CALL(call)                                                   \
    MACRO_START                                                         \
        WM_Status st = (call);                                          \
        if (UNLIKELY((st != WM_STATUS_SUCCESS))) {                      \
            F_LOG_T(OS_STDERR, "WM_CALL",                               \
                    ANSI_COLOR_RED, "call to '"#call"' failed: %s",     \
                    wm_get_status_string(st));                          \
            ABORT();                                                    \
        }                                                               \
    MACRO_END

const char *get_wm_event_type_str(usz event_type);
const char *get_wm_window_event_type_str(usz event_type);
const char *get_wm_keyboard_event_type_str(usz event_type);
const char *get_wm_mouse_event_type_str(usz event_type);
const char *get_wm_mouse_button_str(usz button);
const char *get_wm_keyboard_event_value_str(usz keyboard_value);

const char *wm_get_status_string(usz status);

#endif /* __CORE_WM_UTILS_H__ */
