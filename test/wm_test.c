#include <core/os.h>
#include <core/os_file.h>
#include <core/log.h>
#include <core/wm.h>
#include <core/cstd.h>
#include <core/macros.h>

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

int main(int argc, char **argv)
{
    struct wm window_manager = {0};
    WM_CALL(wm_init(&window_manager));

    struct wm_window main_window = {0};
    struct wm_window_info main_window_info = {
        .width          = 1200,
        .height         = 720,
        .x_pos          = X_POS_CENTERED,
        .y_pos          = Y_POS_CENTERED,
        .force_size     = TRUE,
        .initial_title  = __FILE__,
    };
    WM_CALL(wm_window_create(&window_manager, &main_window, main_window_info));
    wm_window_show(&window_manager, &main_window);
    os_sleep_sec(2);
    WM_CALL(wm_window_close(&window_manager, &main_window));
    WM_CALL(wm_shutdown(&window_manager));

    return EXIT_SUCCESS;
}
