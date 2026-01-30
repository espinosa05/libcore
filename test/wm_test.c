#include <core/os.h>
#include <core/os_file.h>
#include <core/log.h>
#include <core/wm.h>
#include <core/cstd.h>
#include <core/macros.h>

u8 buffer[KB_SIZE*2] = {0};

struct arena {
    u8 *buffer;
    usz cursor;
};

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

/* static function declaration start */
static void handle_window_event(struct wm *window_manager, struct wm_window *window, struct wm_window_event w_event);
static void handle_keyboard_event(struct wm *window_manager, struct wm_window *window, struct wm_keyboard_event k_event);
static void handle_mouse_event(struct wm_mouse_event m_event);
/* static function declaration end */

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

    while (!main_window.should_close) {
        struct wm_event main_window_event = {0};
        wm_window_poll_events(&window_manager, &main_window, &main_window_event);

        switch (main_window_event.event_type) {
        case WM_EVENT_TYPE_WINDOW:
            handle_window_event(&window_manager, &main_window, main_window_event.window_event);
            break;
        case WM_EVENT_TYPE_KEYBOARD:
            handle_keyboard_event(&window_manager, &main_window, main_window_event.key_event);
            break;
        case WM_EVENT_TYPE_MOUSE:
            handle_mouse_event(main_window_event.mouse_event);
            break;
        }
    }

    WM_CALL(wm_window_close(&window_manager, &main_window));
    WM_CALL(wm_shutdown(&window_manager));

    return EXIT_SUCCESS;
}

static void handle_window_event(struct wm *window_manager, struct wm_window *window, struct wm_window_event w_event)
{
    switch (w_event.type) {
    default: INFO_LOG(":(");
    }
}

static void handle_keyboard_event(struct wm *window_manager, struct wm_window *window, struct wm_keyboard_event k_event)
{
    switch (k_event.type) {
    case WM_KEYBOARD_EVENT_TYPE_KEY_PRESS:
        if (k_event.value == 27) {
            window->should_close = TRUE;
        } else {
            INFO_LOG("key pressed : %d", k_event.value);
        }
        break;
    case WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE:
        break;
    }
}

static void handle_mouse_event(struct wm_mouse_event m_event)
{

}

