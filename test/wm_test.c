#include <core/os.h>
#include <core/utils.h>
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

const char *wm_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_EVENT_EMPTY_QUEUE),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_WINDOW),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_KEYBOARD),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_MOUSE),
};

const char *wm_keyboard_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_KEYBOARD_EVENT_TYPE_KEY_PRESS),
    ENUM_STR_ENTRY(WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE),
};

const char *wm_mouse_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_PRESS),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_RELEASE),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_ENTER),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_LEAVE),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_MOTION),
};

const char *wm_window_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_WINDOW_EVENT_TYPE_EXPOSE),
};

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
        .force_size     = FALSE,
        .initial_title  = __FILE__,
    };
    WM_CALL(wm_window_create(&window_manager, &main_window, main_window_info));
    wm_window_show(&window_manager, &main_window);

while (!main_window.should_close) {
        struct wm_event main_window_event = {0};
        wm_window_poll_events(&window_manager, &main_window, &main_window_event);

        if (main_window_event.event_type != 0)
            INFO_LOG(STR_QUOT(STR_FMT), wm_event_type_strs[main_window_event.event_type]);

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
    INFO_LOG(STR_QUOT(STR_FMT), wm_window_event_type_strs[w_event.type]);
}

static void handle_keyboard_event(struct wm *window_manager, struct wm_window *window, struct wm_keyboard_event k_event)
{
    INFO_LOG(STR_QUOT(STR_FMT), wm_window_event_type_strs[k_event.type]);
    INFO_LOG("key code: "USZ_FMT, k_event.value);
    switch (k_event.type) {
    case WM_KEYBOARD_EVENT_TYPE_KEY_PRESS:
        if (k_event.value == 9) {
            window->should_close = TRUE;
        }
        break;
    case WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE:
        break;
    }
}

static void handle_mouse_event(struct wm_mouse_event m_event)
{
    INFO_LOG(STR_QUOT(STR_FMT), wm_mouse_event_type_strs[m_event.type]);

    switch (m_event.type) {
    case WM_MOUSE_EVENT_TYPE_PRESS:
    case WM_MOUSE_EVENT_TYPE_RELEASE:
    case WM_MOUSE_EVENT_TYPE_MOTION:
        INFO_LOG("x: "USZ_FMT, m_event.x_pos);
        INFO_LOG("y: "USZ_FMT, m_event.y_pos);
        break;
    }
}

