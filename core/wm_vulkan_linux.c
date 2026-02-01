#include <core/wm_vulkan.h>
#include <core/utils.h>

void wm_vulkan_extensions_get_required(struct wm_vulkan_extensions *wm_extensions)
{
    static const char *required_xcb_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
    };

    wm_extensions->count = ARRAY_SIZE(required_xcb_extensions);
    wm_extensions->names = required_xcb_extensions;
}

WM_Vulkan_Surface_Status wm_vulkan_surface_create(struct wm_vulkan_surface *surface, const struct wm_vulkan_surface_info info)
{
    CHECK_NULL(info.wm);
    CHECK_NULL(info.win);

    VkResult status = -1;
    VkXcbSurfaceCreateInfoKHR create_info = {
        .sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .connection = info.wm->xcb_connection,
        .window     = info.win->xcb_window,
    };
    VkInstance instance = info.instance;
    status = vkCreateXcbSurfaceKHR(instance, &create_info, NULL, &surface->handle);
    if (status != VK_SUCCESS) {
        return WM_VULKAN_SURFACE_STATUS_FAILED_TO_CREATE_SURFACE;
    }

    return WM_VULKAN_SURFACE_STATUS_SUCCESS;
}

static char *wm_vulkan_surface_status_strs[] = {
    ENUM_STR_ENTRY(WM_VULKAN_SURFACE_STATUS_SUCCESS),
    ENUM_STR_ENTRY(WM_VULKAN_SURFACE_STATUS_FAILED_TO_CREATE_SURFACE),
    ENUM_STR_ENTRY(WM_VULKAN_SURFACE_STATUS_UNKNOWN),
};

char *wm_get_vulkan_surface_status_str(usz status)
{
    if (status > WM_VULKAN_SURFACE_STATUS_COUNT)
        status = WM_VULKAN_SURFACE_STATUS_UNKNOWN;

    return wm_vulkan_surface_status_strs[status];
}
