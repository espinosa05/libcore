#ifndef __CORE_WM_H__
#define __CORE_WM_H__

#include <core/types.h>
#include <core/platform.h>
#include <core/buffer.h>

#if defined(CORE_PLATFORM_LINUX)
#include <xcb/xcb.h>

struct wm {
    xcb_connection_t    *xcb_connection;
    xcb_screen_t        *xcb_screen;
};

struct wm_window {
    b32             should_close;
    xcb_window_t    xcb_window;
    xcb_gcontext_t  xcb_graphics_context;
};

#elif defined(CORE_PLATFORM_WINDOWS)
#error "windows not supported yet"
#endif /* CORE_PLATFOMR_LINUX */

#define X_POS_CENTERED (sz)(-1)
#define Y_POS_CENTERED (sz)(-1)

typedef u32 WM_Status;
enum wm_status_codes {
    WM_STATUS_SUCCESS = 0,
    WM_STATUS_COULDNT_START,
    WM_STATUS_WINDOW_RESOLUTION_NOT_SUPPORTED,

    WM_STATUS_UNKNOWN,
    WM_STATUS_COUNT,
};

struct wm_window_info {
    usz         width;
    usz         height;
    sz          x_pos;
    sz          y_pos;
    b32         force_size;
    const char  *initial_title;
};

WM_Status wm_init(struct wm *wm);
WM_Status wm_shutdown(struct wm *wm);

WM_Status wm_window_create(struct wm *wm, struct wm_window *win, struct wm_window_info info);
void wm_window_show(struct wm *wm, struct wm_window *win);
void wm_window_hide(struct wm *wm, struct wm_window *win);
void wm_window_change_title(struct wm *wm, struct wm_window *win, const char *title);
void wm_window_force_size(struct wm *wm, struct wm_window *win, usz width, usz height);
void wm_window_grab_mouse(struct wm *wm, struct wm_window *win);
void wm_window_release_mouse(struct wm *wm, struct wm_window *win);
WM_Status wm_window_close(struct wm *wm, struct wm_window *win);
const char *wm_get_status_string(usz status);

enum wm_event_types {
    WM_EVENT_EMPTY_QUEUE = 0,
    WM_EVENT_TYPE_WINDOW,
    WM_EVENT_TYPE_KEYBOARD,
    WM_EVENT_TYPE_MOUSE,
};

enum wm_keyboard_event_types {
    WM_KEYBOARD_EVENT_TYPE_KEY_PRESS,
    WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE,
};

enum wm_mouse_event_types {
    WM_MOUSE_EVENT_TYPE_PRESS,
    WM_MOUSE_EVENT_TYPE_RELEASE,
    WM_MOUSE_EVENT_TYPE_ENTER,
    WM_MOUSE_EVENT_TYPE_LEAVE,
    WM_MOUSE_EVENT_TYPE_MOTION,
};

enum wm_window_event_types {
    WM_WINDOW_EVENT_TYPE_EXPOSE,
};

struct wm_window_expose_event {
    usz x;
    usz y;
    usz width;
    usz height;
};

struct wm_window_event {
    usz type;
    union {
        struct wm_window_expose_event expose;
    };
};

struct wm_keyboard_event {
    usz type;
    u64 value;
};

enum mouse_buttons {
    WM_MOUSE_BUTTON_LEFT = 1,
    WM_MOUSE_BUTTON_MIDDLE = 2,
    WM_MOUSE_BUTTON_RIGHT = 3,
    WM_MOUSE_BUTTON_SCROLL_UP = 4,
    WM_MOUSE_BUTTON_SCROLL_DOWN = 5,
};

struct wm_mouse_event {
    usz type;
    usz value;
    usz x_pos;
    usz y_pos;
};

struct wm_event {
    usz event_type;
    union {
        struct wm_window_event window_event;
        struct wm_keyboard_event key_event;
        struct wm_mouse_event mouse_event;
    };
};

void wm_window_poll_events(struct wm *wm, struct wm_window *win, struct wm_event *event);

#endif /* __CORE_WM_H__ */
