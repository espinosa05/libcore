#ifndef __CORE_WM_H__
#define __CORE_WM_H__

#include <core/types.h>
#include <core/platform.h>

#if defined(CORE_PLATFORM_LINUX)
#include <xcb/xcb.h>

struct wm {
    xcb_connection_t    *xcb_connection;
    xcb_screen_t        *xcb_screen;
};

struct wm_window {
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
WM_Status wm_window_close(struct wm *wm, struct wm_window *win);


const char *wm_get_status_string(usz status);
#endif /* __CORE_WM_H__ */
