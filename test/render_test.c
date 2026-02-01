#include <core/os.h>
#include <core/utils.h>
#include <core/os_file.h>
#include <core/log.h>
#include <core/wm.h>
#include <core/wm_utils.h>
#include <core/wm_vulkan.h>
#include <core/cstd.h>
#include <core/macros.h>

#include <kiek/kiek_vulkan_backend.h>

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

    struct wm_vulkan_extensions extensions = {0};
    wm_vulkan_extensions_get_required(&extensions);

    struct kiek_vulkan_context renderer = {0};
    struct kiek_vulkan_context_info renderer_info = {
        .version    = KIEK_APP_VERSION_HEADER_REF(0, 0, 1),
        .app_name   = __FILE__,
        .extensions = &extensions,
    };
    kiek_vulkan_startup(&renderer, renderer_info);

    struct wm_vulkan_surface surface = {0};
    struct wm_vulkan_surface_info surface_info = {
        .wm         = &window_manager,
        .win        = &main_window,
        .instance   = kiek_vulkan_get_instance(renderer),
    };
    wm_vulkan_surface_create(&surface, surface_info);

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

    kiek_vulkan_shutdown(&renderer);
    WM_CALL(wm_window_close(&window_manager, &main_window));
    WM_CALL(wm_shutdown(&window_manager));

    return EXIT_SUCCESS;
}

static void handle_window_event(struct wm *window_manager, struct wm_window *window, struct wm_window_event w_event)
{

}

static void handle_keyboard_event(struct wm *window_manager, struct wm_window *window, struct wm_keyboard_event k_event)
{

}

static void handle_mouse_event(struct wm_mouse_event m_event)
{

}
