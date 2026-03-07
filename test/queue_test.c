#include <core/types.h>
#include <core/log.h>
#include <core/os.h>
#include <core/os_thread.h>
#include <core/memory_macros.h>
#include <core/memory.h>

struct usz_queue {
    DECL_MM_QUEUE_MEMBERS(usz);
};

#define INIT_CAP 32

int main(int argc, char **argv)
{
    struct usz_queue queue = {0};
    mm_queue_init(&queue, 4);

    for (usz i = 0; i < 16; ++i) {
        LOG("E --> : "USZ_FMT STR_NL, i);
        mm_queue_enqueue(&queue, i);
    }

    usz e = 0;
    LOG(STR_NL);
    mm_queue_dequeue(&queue, &e);
    LOG("<-- D : "USZ_FMT STR_NL, e);
    mm_queue_dequeue(&queue, &e);
    LOG("<-- D : "USZ_FMT STR_NL, e);

    LOG(STR_NL);
    LOG("E --> : "USZ_FMT STR_NL, 512);
    mm_queue_enqueue(&queue, 512);

    LOG(STR_NL);

    mm_queue_dequeue(&queue, &e);
    LOG("<-- D : "USZ_FMT STR_NL, e);

    LOG(STR_NL);
    for (usz i = 0; i < mm_queue_length(&queue); ++i) {
        mm_queue_dequeue(&queue, &e);
        LOG("<-- D : "USZ_FMT STR_NL, e);
        os_thread_sleep_msec(100);
    }

    return OS_EXIT_SUCCESS;
}
