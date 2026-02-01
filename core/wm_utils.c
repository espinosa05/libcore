#include <core/wm_utils.h>
#include <core/wm.h>

static const char *wm_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_EVENT_EMPTY_QUEUE),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_WINDOW),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_KEYBOARD),
    ENUM_STR_ENTRY(WM_EVENT_TYPE_MOUSE),
};

static const char *wm_keyboard_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_KEYBOARD_EVENT_TYPE_KEY_PRESS),
    ENUM_STR_ENTRY(WM_KEYBOARD_EVENT_TYPE_KEY_RELEASE),
};

static const char *wm_mouse_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_PRESS),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_RELEASE),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_ENTER),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_LEAVE),
    ENUM_STR_ENTRY(WM_MOUSE_EVENT_TYPE_MOTION),
};

static const char *wm_window_event_type_strs[] = {
    ENUM_STR_ENTRY(WM_WINDOW_EVENT_TYPE_EXPOSE),
};

static const char *wm_mouse_button_strs[] = {
    "EXTRA_KEY",
    ENUM_STR_ENTRY(WM_MOUSE_BUTTON_LEFT),
    ENUM_STR_ENTRY(WM_MOUSE_BUTTON_MIDDLE),
    ENUM_STR_ENTRY(WM_MOUSE_BUTTON_RIGHT),
    ENUM_STR_ENTRY(WM_MOUSE_BUTTON_SCROLL_UP),
    ENUM_STR_ENTRY(WM_MOUSE_BUTTON_SCROLL_DOWN),
};

static const char *wm_keysyms[] = {
    ENUM_STR_ENTRY(WM_KEYSYM_A),
    ENUM_STR_ENTRY(WM_KEYSYM_B),
    ENUM_STR_ENTRY(WM_KEYSYM_C),
    ENUM_STR_ENTRY(WM_KEYSYM_D),
    ENUM_STR_ENTRY(WM_KEYSYM_E),
    ENUM_STR_ENTRY(WM_KEYSYM_F),
    ENUM_STR_ENTRY(WM_KEYSYM_G),
    ENUM_STR_ENTRY(WM_KEYSYM_H),
    ENUM_STR_ENTRY(WM_KEYSYM_I),
    ENUM_STR_ENTRY(WM_KEYSYM_J),
    ENUM_STR_ENTRY(WM_KEYSYM_K),
    ENUM_STR_ENTRY(WM_KEYSYM_L),
    ENUM_STR_ENTRY(WM_KEYSYM_M),
    ENUM_STR_ENTRY(WM_KEYSYM_N),
    ENUM_STR_ENTRY(WM_KEYSYM_O),
    ENUM_STR_ENTRY(WM_KEYSYM_P),
    ENUM_STR_ENTRY(WM_KEYSYM_Q),
    ENUM_STR_ENTRY(WM_KEYSYM_R),
    ENUM_STR_ENTRY(WM_KEYSYM_S),
    ENUM_STR_ENTRY(WM_KEYSYM_T),
    ENUM_STR_ENTRY(WM_KEYSYM_U),
    ENUM_STR_ENTRY(WM_KEYSYM_V),
    ENUM_STR_ENTRY(WM_KEYSYM_W),
    ENUM_STR_ENTRY(WM_KEYSYM_X),
    ENUM_STR_ENTRY(WM_KEYSYM_Y),
    ENUM_STR_ENTRY(WM_KEYSYM_Z),

    ENUM_STR_ENTRY(WM_KEYSYM_BACKSPACE),
    ENUM_STR_ENTRY(WM_KEYSYM_TAB),
    ENUM_STR_ENTRY(WM_KEYSYM_ENTER),
    ENUM_STR_ENTRY(WM_KEYSYM_ESC),

    ENUM_STR_ENTRY(WM_KEYSYM_SHIFT_L),
    ENUM_STR_ENTRY(WM_KEYSYM_SHIFT_R),
    ENUM_STR_ENTRY(WM_KEYSYM_CTRL_L),
    ENUM_STR_ENTRY(WM_KEYSYM_CTRL_R),
    ENUM_STR_ENTRY(WM_KEYSYM_CAPS_LOCK),
    ENUM_STR_ENTRY(WM_KEYSYM_ALT_L),
    ENUM_STR_ENTRY(WM_KEYSYM_ALT_R),
    ENUM_STR_ENTRY(WM_KEYSYM_SUPER_L),
    ENUM_STR_ENTRY(WM_KEYSYM_SUPER_R),
};

const char *get_wm_event_type_str(usz event_type)
{
    if (event_type >= ARRAY_SIZE(wm_event_type_strs))
        return "WM_EVENT_TYPE_UNSUPPORTED";

    const char *type = wm_event_type_strs[event_type];
    if (!type)
        return "WM_EVENT_TYPE_UNSUPPORTED";

    return type;
}

const char *get_wm_window_event_type_str(usz event_type)
{
    ASSERT(event_type < ARRAY_SIZE(wm_window_event_type_strs), "!");
    return wm_window_event_type_strs[event_type];
}

const char *get_wm_keyboard_event_type_str(usz event_type)
{
    ASSERT(event_type < ARRAY_SIZE(wm_keyboard_event_type_strs), "!");
    return wm_keyboard_event_type_strs[event_type];
}

const char *get_wm_mouse_event_type_str(usz event_type)
{
    ASSERT(event_type < ARRAY_SIZE(wm_mouse_event_type_strs), "!");
    return wm_mouse_event_type_strs[event_type];
}

const char *get_wm_mouse_button_str(usz button)
{
    if (button > WM_MOUSE_BUTTON_SCROLL_DOWN)
        button = 0;

    return wm_mouse_button_strs[button];
}

const char *get_wm_keyboard_event_value_str(usz keyboard_value)
{
    if (keyboard_value > ARRAY_SIZE(wm_keysyms))
        return "WM_KEY_UNSUPPORTED";

    const char *key_value = wm_keysyms[keyboard_value];
    if (!key_value)
        return "WM_KEY_UNSUPPORTED";

    return key_value;
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
