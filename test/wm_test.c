#include <core/os.h>
#include <core/utils.h>
#include <core/os_file.h>
#include <core/log.h>
#include <core/wm.h>
#include <core/wm_utils.h>
#include <core/cstd.h>
#include <core/macros.h>

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
    INFO_LOG(STR_QUOT(STR_FMT), get_wm_window_event_type_str(w_event.type));
}

static void handle_keyboard_event(struct wm *window_manager, struct wm_window *window, struct wm_keyboard_event k_event)
{
    INFO_LOG(STR_QUOT(STR_FMT), get_wm_keyboard_event_type_str(k_event.type));
    INFO_LOG("key code: "STR_FMT, get_wm_keyboard_event_value_str(k_event.value));
    switch (k_event.type) {
    case WM_KEYBOARD_EVENT_TYPE_KEY_PRESS:
        switch (k_event.value) {
        case WM_KEYSYM_ESC:
        case WM_KEYSYM_Q:
            window->should_close = TRUE;
        }
        break;
    case WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE:
        break;
    }
}

static void handle_mouse_event(struct wm_mouse_event m_event)
{
    INFO_LOG(STR_QUOT(STR_FMT), get_wm_mouse_event_type_str(m_event.type));
    INFO_LOG("x: "USZ_FMT, m_event.x_pos);
    INFO_LOG("y: "USZ_FMT, m_event.y_pos);

    switch (m_event.type) {
    case WM_MOUSE_EVENT_TYPE_PRESS:
    case WM_MOUSE_EVENT_TYPE_RELEASE:
        INFO_LOG("button_nr :"USZ_FMT, m_event.value);
        INFO_LOG("button :"STR_FMT, get_wm_mouse_button_str(m_event.value));
        break;
    case WM_MOUSE_EVENT_TYPE_MOTION:
        break;
    }
}
