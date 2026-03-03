#include <core/macros.h>
#include <core/cpu.h>
#include <core/memory.h>
#include <core/types.h>
#include <core/log.h>
#include <core/os.h>
#include <core/os_thread.h>

static struct os_mutex g_print_mutex = {0};

#ifdef TT_PRINT
# define TS_INFO_LOG(...)               \
    __MACRO_START__                     \
        os_mutex_lock(&g_print_mutex);  \
        INFO_LOG(__VA_ARGS__);          \
        os_mutex_unlock(&g_print_mutex);\
    __MACRO_END__

#else
# define TS_INFO_LOG(...) __EMPTY_MACRO__
#endif

void test_job(void *id)
{
    TS_INFO_LOG("hello from thread "USZ_FMT, USZ(id));
}

#define BUFF_SIZE (KB_SIZE*128)
int main(int argc, char **argv)
{
    struct m_arena arena = {0};
    struct m_arena_info arena_info = {0};
    arena_info.mem_size = BUFF_SIZE;
    m_arena_init(&arena, arena_info);

    struct cpu_info cpu_info = {0};
    cpu_info_query_all(&cpu_info, NULL);
    usz thread_count = 3;

    struct m_array threads = {0};
    struct m_array_info threads_info = {0};
    threads_info.base   = m_arena_alloc(&arena, sizeof(struct os_thread), thread_count);
    threads_info.width  = sizeof(struct os_thread);
    threads_info.cap    = thread_count;
    threads_info.count  = thread_count;
    m_array_init_ext(&threads, threads_info);

    if (0 != os_mutex_init(&g_print_mutex)) {
        return OS_EXIT_FAILURE;
    }

    for (usz id = 0; id < thread_count; ++id) {
        struct os_thread *thread = m_array_get_addr(&threads, id);
        TS_INFO_LOG("spawning thread: "USZ_FMT, id);
        os_thread_spawn(thread, test_job, PTR(id));
    }

    for (usz id = thread_count; id-- > 0; ) {
        struct os_thread *thread = TYPE_ARRAY_ELEMENT(struct os_thread, threads.data, id);
        TS_INFO_LOG("joining thread: "USZ_FMT, id);
        os_thread_join(thread);
    }

    m_arena_destroy(&arena);
    return OS_EXIT_SUCCESS;
}
