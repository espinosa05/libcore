#ifndef __CORE_WM_VULKAN_H__
#define __CORE_WM_VULKAN_H__

#include <core/wm.h>
#include <core/vulkan.h>

struct wm_vulkan_extensions {
    u32         count;
    const char  **names;
};

struct wm_vulkan_surface {
    VkSurfaceKHR handle;
};

struct wm_vulkan_surface_info {
    VkInstance          instance;
    struct wm           *wm;
    struct wm_window    *win;
};

typedef u32 WM_Vulkan_Surface_Status;
enum wm_vulkan_surface_status_codes {
    WM_VULKAN_SURFACE_STATUS_SUCCESS = 0,
    WM_VULKAN_SURFACE_STATUS_FAILED_TO_CREATE_SURFACE,

    WM_VULKAN_SURFACE_STATUS_COUNT,
#define WM_VULKAN_SURFACE_STATUS_UNKNOWN WM_VULKAN_SURFACE_STATUS_COUNT
};

void wm_vulkan_extensions_get_required(struct wm_vulkan_extensions *wm_extensions);
WM_Vulkan_Surface_Status wm_vulkan_surface_create(struct wm_vulkan_surface *surface, const struct wm_vulkan_surface_info info);
char *wm_get_vulkan_surface_status_str(usz status);

#endif /* __V2_CORE_WM_VULKAN_H__ */
