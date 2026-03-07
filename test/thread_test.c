#include <core/macros.h>
#include <core/cpu.h>
#include <core/memory.h>
#include <core/types.h>
#include <core/log.h>
#include <core/os.h>
#include <core/os_thread.h>
#include <core/memory_macros.h>

static struct os_mutex g_print_mutex = {0};

struct threads {
    MM_ARRAY_MEMBERS(struct os_thread);
};

#ifdef TT_PRINT
# define TS_INFO_LOG(...)               \
    MACRO_START                         \
        os_mutex_lock(&g_print_mutex);  \
        INFO_LOG(__VA_ARGS__);          \
        os_mutex_unlock(&g_print_mutex);\
    MACRO_END

#else
# define TS_INFO_LOG(...) __EMPTY_MACRO__
#endif

void test_job(void *id)
{
    TS_INFO_LOG("hello from thread "USZ_FMT, USZ(id));
}

#define BUFF_SIZE (KB_SIZE*2)
int main(int argc, char **argv)
{
    struct cpu_info cpu_info = {0};
    cpu_info_query_all(&cpu_info, NULL);

    struct m_arena arena = {0};
    struct m_arena_info arena_info = {0};
    arena_info.mem_size = BUFF_SIZE;
    arena_info.buffer   = (u8 [BUFF_SIZE]) {0};
    arena_info.external = TRUE;
    m_arena_init(&arena, arena_info);

    usz count = 10;
    usz buff_size = sizeof(struct threads) * count;
    ASSERT(buff_size <= BUFF_SIZE, "thread buffer is not gonna fit inside arena memory!");
    struct threads threads = {0};
    mm_array_init_ext(&threads, m_arena_alloc(&arena, BYTE_SIZE, buff_size), count);

    INFO_LOG("initializing print mutex...");
    if (0 != os_mutex_init(&g_print_mutex)) {
        return OS_EXIT_FAILURE;
    }

    TS_INFO_LOG("initializing threads...");
    for (usz id = 0; id < threads.cap; ++id) {
        TS_INFO_LOG("spawning thread: "USZ_FMT, id);
        os_thread_spawn(&threads.data[id], test_job, PTR(id));
    }

    for (usz id = threads.cap; id-- > 0; ) {
        TS_INFO_LOG("joining thread: "USZ_FMT, id);
        os_thread_join(&threads.data[id]);
    }

    return OS_EXIT_SUCCESS;
}
