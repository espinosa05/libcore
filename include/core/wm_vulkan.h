#ifndef __CORE_WM_VULKAN_H__
#define __CORE_WM_VULKAN_H__

#include <core/wm.h>
#include <core/vulkan.h>

struct wm_extensions {
    u32         count;
    const char  **names;
};

struct wm_surface {
    VkSurfaceKHR handle;
};

struct wm_surface_info {
    VkInstance          *instance;
    struct wm           *wm;
    struct wm_window    *win;
};

typedef u32 WM_Surface_Status;
enum wm_surface_status_codes {
    WM_SURFACE_STATUS_SUCCESS = 0,
    WM_SURFACE_STATUS_FAILED_TO_CREATE_SURFACE,

    WM_SURFACE_STATUS_COUNT,
#define WM_SURFACE_STATUS_UNKNOWN WM_SURFACE_STATUS_COUNT
};

void wm_get_required_vulkan_extensions(struct wm_extensions *wm_extensions);
void wm_cleanup_required_vulkan_extensions(const struct wm_extensions wm_extensions);
WM_Surface_Status wm_surface_create(struct wm_surface *surface, const struct wm_surface_info info);
char *wm_surface_status_string(usz status);

#endif /* __V2_CORE_WM_VULKAN_H__ */
