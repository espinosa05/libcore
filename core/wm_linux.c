#include <core/wm.h>
#include <core/log.h>
#include <core/wm_vulkan.h>
#include <core/utils.h>
#include <core/memory.h>

#include <xcb/xcb_icccm.h>

/* static function declaration start */
static b32 supported_window_resolution(const struct wm_window_info info);
/* static function declaration end */

WM_Status wm_init(struct wm *wm)
{
    xcb_setup_t             *xcb_setup;
    xcb_screen_iterator_t   xcb_screens;

    wm->xcb_connection   = xcb_connect(NULL, NULL);
    xcb_setup            = (xcb_setup_t *)xcb_get_setup(wm->xcb_connection);
    xcb_screens          = xcb_setup_roots_iterator(xcb_setup);
    wm->xcb_screen       = xcb_screens.data;

    return WM_STATUS_SUCCESS;
}

WM_Status wm_shutdown(struct wm *wm)
{
    xcb_disconnect(wm->xcb_connection);

    return WM_STATUS_SUCCESS;
}

#define DEFAULT_BORDER_WIDTH        0
#define DEFAULT_BORDER_COLOR        0
#define DEFAULT_BACKGROUND_COLOR    0

WM_Status wm_window_create(struct wm *wm, struct wm_window *win, struct wm_window_info info)
{

    if (info.x_pos == X_POS_CENTERED)
        info.x_pos = wm->xcb_screen->width_in_pixels;

    if (info.y_pos == Y_POS_CENTERED)
        info.y_pos = wm->xcb_screen->height_in_pixels;

    if (!supported_window_resolution(info))
        return WM_STATUS_WINDOW_RESOLUTION_NOT_SUPPORTED;

    win->xcb_window = xcb_generate_id(wm->xcb_connection);
    xcb_create_window(wm->xcb_connection,
                      XCB_COPY_FROM_PARENT,
                      win->xcb_window,
                      wm->xcb_screen->root,
                      info.x_pos,
                      info.y_pos,
                      info.width,
                      info.height,
                      DEFAULT_BORDER_WIDTH,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      wm->xcb_screen->root_visual,
                      0,
                      NULL);

    wm_window_change_title(wm, win, info.initial_title);

    if (info.force_size)
        wm_window_force_size(wm, win, info.width, info.height);

    u32 event_values[] = { XCB_EVENT_MASK_EXPOSURE, XCB_EVENT_MASK_KEY_PRESS, XCB_EVENT_MASK_KEY_RELEASE, };
    xcb_change_window_attributes(wm->xcb_connection, win->xcb_window, XCB_CW_EVENT_MASK, event_values);

    xcb_flush(wm->xcb_connection);

    return WM_STATUS_SUCCESS;
}

void wm_window_show(struct wm *wm, struct wm_window *win)
{
    xcb_map_window(wm->xcb_connection, win->xcb_window);
    xcb_flush(wm->xcb_connection);
}

void wm_window_hide(struct wm *wm, struct wm_window *win)
{
    xcb_unmap_window(wm->xcb_connection, win->xcb_window);
    xcb_flush(wm->xcb_connection);
}

#define UTF8_WIDTH 8

void wm_window_change_title(struct wm *wm, struct wm_window *win, const char *title)
{
    xcb_intern_atom_cookie_t xcb_cookie;
    xcb_intern_atom_reply_t *xcb_reply;
    xcb_atom_t window_name;
    xcb_atom_t utf8_string;
    usz title_length;

    {
        xcb_cookie  = xcb_intern_atom(wm->xcb_connection, 0, CONST_STRLEN("_NET_WM_NAME"), "_NET_WM_NAME");
        xcb_reply   = xcb_intern_atom_reply(wm->xcb_connection, xcb_cookie, NULL);
        window_name = xcb_reply->atom;
        m_free(xcb_reply);
    }

    {
        xcb_cookie  = xcb_intern_atom(wm->xcb_connection, 0, CONST_STRLEN("UTF8_STRING"), "UTF8_STRING");
        xcb_reply   = xcb_intern_atom_reply(wm->xcb_connection, xcb_cookie, NULL);
        utf8_string = xcb_reply->atom;
        m_free(xcb_reply);
    }

    title_length = cstr_length(title);
    xcb_change_property(wm->xcb_connection,
                        XCB_PROP_MODE_REPLACE,
                        win->xcb_window,
                        window_name,
                        utf8_string,
                        UTF8_WIDTH,
                        title_length,
                        title);

    xcb_change_property(wm->xcb_connection,
                        XCB_PROP_MODE_REPLACE,
                        win->xcb_window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        UTF8_WIDTH,
                        title_length,
                        title);

    xcb_flush(wm->xcb_connection);
}

void wm_window_force_size(struct wm *wm, struct wm_window *win, usz width, usz height)
{
    xcb_size_hints_t size_hints = {0};

    xcb_icccm_size_hints_set_min_size(&size_hints, width, height);
    xcb_icccm_size_hints_set_max_size(&size_hints, width, height);
    xcb_icccm_size_hints_set_base_size(&size_hints, width, height);
    xcb_icccm_size_hints_set_resize_inc(&size_hints, 0, 0);
    xcb_icccm_set_wm_size_hints(wm->xcb_connection, win->xcb_window, XCB_ATOM_WM_NORMAL_HINTS, &size_hints);
}

WM_Status wm_window_close(struct wm *wm, struct wm_window *win)
{
    xcb_destroy_window(wm->xcb_connection, win->xcb_window);
    xcb_flush(wm->xcb_connection);

    return WM_STATUS_SUCCESS;
}

void wm_get_required_extensions(struct wm_extensions *wm_extensions)
{
    static const char *required_xcb_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
    };

    wm_extensions->count = ARRAY_SIZE(required_xcb_extensions);
    wm_extensions->names = required_xcb_extensions;
}

WM_Surface_Status wm_surface_create(struct wm_surface *surface, const struct wm_surface_info info)
{
    CHECK_NULL(info.wm);
    CHECK_NULL(info.win);

    VkResult status = -1;
    VkXcbSurfaceCreateInfoKHR create_info = {
        .sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .connection = info.wm->xcb_connection,
        .window     = info.win->xcb_window,
    };
    VkInstance instance = *info.instance;
    status = vkCreateXcbSurfaceKHR(instance, &create_info, NULL, &surface->handle);
    if (status != VK_SUCCESS) {
        return WM_SURFACE_STATUS_FAILED_TO_CREATE_SURFACE;
    }

    return WM_SURFACE_STATUS_SUCCESS;
}

#define GET_XCB_RESPONSE_TYPE(e) ((e)->response_type & ~0x80)
void wm_window_poll_events(struct wm *wm, struct wm_window *win, struct wm_event *event)
{
    CHECK_NULL(wm);
    CHECK_NULL(win);
    CHECK_NULL(event);

    xcb_generic_event_t *xcb_event = xcb_poll_for_event(wm->xcb_connection);
    if (!xcb_event) {
        event->event_type = WM_EVENT_EMPTY_QUEUE;
        return;
    }

    INFO_LOG("xcb_event->response_type: %d", GET_XCB_RESPONSE_TYPE(xcb_event));

    switch (GET_XCB_RESPONSE_TYPE(xcb_event)) {
    case XCB_EXPOSE: {
        xcb_expose_event_t *xcb_expose_event = (xcb_expose_event_t *)xcb_event;
        if (xcb_expose_event->count != 0) {
            break;
        }
        event->event_type = WM_EVENT_TYPE_WINDOW;
        event->window_event.type = WM_WINDOW_EVENT_TYPE_EXPOSE;
        event->window_event.expose.x = xcb_expose_event->x;
        event->window_event.expose.y = xcb_expose_event->y;
        event->window_event.expose.width = xcb_expose_event->width;
        event->window_event.expose.height = xcb_expose_event->height;
    } break;
    case XCB_KEY_PRESS: {
        xcb_key_press_event_t *xcb_key_press_event = (xcb_key_press_event_t *)xcb_event;
        event->event_type = WM_EVENT_TYPE_KEYBOARD;
        event->key_event.type = WM_KEYBOARD_EVENT_TYPE_KEY_PRESS;
        event->key_event.value = xcb_key_press_event->detail;
    } break;
    case XCB_KEY_RELEASE: {
        xcb_key_release_event_t *xcb_key_release_event = (xcb_key_release_event_t *)xcb_event;
        event->event_type = WM_EVENT_TYPE_KEYBOARD;
        event->key_event.type = WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE;
        event->key_event.value = xcb_key_release_event->detail;
    } break;
    default: {
        INFO_LOG("processed xcb event -> 0x%x", event->event_type);
    } break;
    }

    m_free(xcb_event);
}

static const char *wm_status_str[] = {
    ENUM_STR_ENTRY(WM_STATUS_SUCCESS),
    ENUM_STR_ENTRY(WM_STATUS_COULDNT_START),
    ENUM_STR_ENTRY(WM_STATUS_WINDOW_RESOLUTION_NOT_SUPPORTED),
    ENUM_STR_ENTRY(WM_STATUS_UNKNOWN),
};

const char *wm_get_status_string(usz status)
{
    if (status >= WM_STATUS_COUNT)
        status = WM_STATUS_UNKNOWN;

    return wm_status_str[status];
}

static b32 supported_window_resolution(const struct wm_window_info info)
{
    UNUSED(info);
    return TRUE;
}

