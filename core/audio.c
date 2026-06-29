#include <core/audio.h>
#include <core/math.h>
#include <core/memory.h>
#include <core/utils.h>

#undef UNUSED
#define UNUSED(a) (void)a

/* static function declaration start */
static void on_process(void *userdata);
/* static function declaration end */

#define POD_BUFFER_SIZE     1024
#define DEFAULT_CHANNELS    2
#define DEFAULT_RATE        44100
#define DEFAULT_VOLUME      F64(0.7)

Audio_Status audio_init(struct audio_context *audio, struct audio_context_info info)
{
    UNUSED(info);

    static struct pw_stream_events stream_events = {0};
    stream_events.version   = PW_VERSION_STREAM_EVENTS;
    stream_events.process   = on_process;

    const struct spa_pod *params[1];

    pw_init(0, NULL);

    u8 *buffer = m_arena_alloc(info.arena, BYTE_SIZE, POD_BUFFER_SIZE);
    struct spa_pod_builder pod_builder = SPA_POD_BUILDER_INIT(buffer, POD_BUFFER_SIZE);

    audio->pw_main = pw_main_loop_new(NULL);

    /* create stream */
    struct pw_loop *main = pw_main_loop_get_loop(audio->pw_main);
    struct pw_properties *properties = pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio", PW_KEY_MEDIA_CATEGORY, "Playback", PW_KEY_MEDIA_ROLE, "Music", NULL);
    audio->pw_stream = pw_stream_new_simple(main, "audio-src", properties, &stream_events, audio);

    struct spa_audio_info_raw audio_info_raw = {0};
    audio_info_raw.format   = SPA_AUDIO_FORMAT_S16;
    audio_info_raw.channels = DEFAULT_CHANNELS;
    audio_info_raw.rate     = DEFAULT_RATE;

    params[0] = spa_format_audio_raw_build(&pod_builder, SPA_PARAM_EnumFormat, &audio_info_raw);

    /* connect to server */
    u32 flags = PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS;
    pw_stream_connect(audio->pw_stream, PW_DIRECTION_OUTPUT, PW_ID_ANY, flags, params, ARRAY_SIZE(params));

    /* run client */
    pw_main_loop_run(audio->pw_main);

    return AUDIO_STATUS_SUCCESS;
}

static char *g_audio_status_strs[] = {
    ENUM_STR_ENTRY(AUDIO_STATUS_SUCCESS),
    ENUM_STR_ENTRY(AUDIO_STATUS_UNKNOWN),
};

char *audio_status_str(usz st)
{
    if (st > AUDIO_STATUS_COUNT)
        st = AUDIO_STATUS_UNKNOWN;

    return g_audio_status_strs[st];
}

void audio_shutdown(struct audio_context *audio)
{
    pw_stream_destroy(audio->pw_stream);
    pw_main_loop_destroy(audio->pw_main);

    pw_deinit();
}

#define CORE_PI_M2 (CORE_PI + CORE_PI)

static void on_process(void *userdata)
{
    struct audio_context *audio = userdata;

    s16 *dst = NULL;
    usz stride = 0;
    usz n_frames = 0;
    struct pw_buffer *pw_buffer = NULL;
    struct spa_buffer *spa_buff = NULL;

    pw_buffer = pw_stream_dequeue_buffer(audio->pw_stream);
    if (!pw_buffer) {
        WARN_LOG("out of buffers");
        return;
    }

    spa_buff = pw_buffer->buffer;
    dst = spa_buff->datas[0].data;
    if (!dst) {
        return;
    }

    stride = sizeof(u16) * DEFAULT_CHANNELS;
    n_frames = spa_buff->datas[0].maxsize / stride;
    if (pw_buffer->requested) {
        n_frames = CORE_MIN(pw_buffer->requested, n_frames);
    }

    for (usz i = 0; i < n_frames; ++i) {
        u16 val = 0;
        audio->acc += CORE_PI_M2 * 440 / DEFAULT_RATE;
        if (audio->acc >= CORE_PI_M2) {
            audio->acc -= CORE_PI_M2;
        }

        val = core_sin(audio->acc) * DEFAULT_VOLUME * F64(S16_MAX);
        for (usz channel = 0; channel < DEFAULT_CHANNELS; ++channel) {
            *dst++ = val;
        }
    }

    spa_buff->datas[0].chunk->offset = 0;
    spa_buff->datas[0].chunk->stride = stride;
    spa_buff->datas[0].chunk->size   = n_frames * stride;

    pw_stream_queue_buffer(audio->pw_stream, pw_buffer);
}

