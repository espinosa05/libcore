#ifndef __CORE_AUDIO_H__
#define __CORE_AUDIO_H__

#include <core/platform.h>
#include <core/math.h>
#include <core/types.h>
#include <core/macros.h>
#include <core/utils.h>
#include <core/log.h>

#if defined(CORE_PLATFORM_LINUX)
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>

struct audio_context {
    f64 acc;

    struct pw_core      *core;
    struct pw_main_loop *pw_main;
    struct pw_stream    *pw_stream;
};
#else
# error "Platform not supported"
#endif /* CORE_PLATFORM_LINUX */

struct audio_context_info {
    struct m_arena *arena;
};

enum audio_status_codes {
    AUDIO_STATUS_SUCCESS = 0,
    AUDIO_STATUS_COUNT,
#define AUDIO_STATUS_UNKNOWN AUDIO_STATUS_COUNT
};
typedef usz Audio_Status;

#define AUDIO_CALL(call)                                                        \
    MACRO_START                                                                 \
        usz st = (call);                                                        \
        if (UNLIKELY(st != AUDIO_STATUS_SUCCESS)) {                             \
            F_LOG_T(OS_STDERR, "AUDIO", "call to '"#call"' failed: "STR_FMT,    \
                    audio_status_str(st));                                      \
            ABORT();                                                            \
        }                                                                       \
    MACRO_END

Audio_Status audio_init(struct audio_context *audio, struct audio_context_info info);
char *audio_status_str(usz st);
void audio_shutdown(struct audio_context *audio);

#endif /* __CORE_AUDIO_H__ */
