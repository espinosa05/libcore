#include <core/types.h>
#include <core/log.h>
#include <core/os.h>
#include <core/memory_macros.h>
#include <core/memory.h>


#define INIT_CAP 32

#define MM_QUEUE_MEMBERS(type)  \
    usz cap;                    \
    usz l_bounds;               \
    usz u_bounds;               \
    type *data

struct usz_queue {
    MM_QUEUE_MEMBERS(usz);
};

#define mm_queue_init(mm_q, init_cap)                                       \
    (mm_q)->cap         = init_cap;                                         \
    (mm_q)->l_bounds    = 0;                                                \
    (mm_q)->u_bounds    = 0;                                                \
    (mm_q)->data        = m_alloc(sizeof(*(mm_q)->data), (mm_q)->cap)

#define DIFF(a, b)  ((a) > (b)) ? (a) - (b) : (b) - (a)
#define mm_queue_length(mm_q) DIFF((mm_q)->l_bounds, (mm_q)->u_bounds)

#define mm_queue_enqueue(mm_q, e)                                                   \
    if (mm_queue_length(mm_q) + 1> (mm_q)->cap) {                                   \
        LOG(STR_NL);                                                                \
        LOG("RESIZING TO "USZ_FMT STR_NL, (mm_q)->cap * 2);                         \
        LOG(STR_NL);                                                                \
        (mm_q)->cap *= 2;                                                           \
        (mm_q)->data = m_realloc((mm_q)->data, sizeof(*(mm_q)->data), (mm_q)->cap); \
    }                                                                               \
    (mm_q)->data[(mm_q)->u_bounds++] = e

#define mm_queue_dequeue(mm_q)               \
    (mm_q)->data[(mm_q)->l_bounds++]

int main(int argc, char **argv)
{
    struct usz_queue queue = {0};
    mm_queue_init(&queue, 4);

    for (usz i = 0; i < 16; ++i) {
        LOG("E --> : "USZ_FMT STR_NL, i);
        mm_queue_enqueue(&queue, i);
    }

    LOG(STR_NL);
    LOG("<-- D : "USZ_FMT STR_NL, mm_queue_dequeue(&queue));
    LOG("<-- D : "USZ_FMT STR_NL, mm_queue_dequeue(&queue));

    LOG(STR_NL);
    LOG("E --> : "USZ_FMT STR_NL, 512);
    mm_queue_enqueue(&queue, 512);

    LOG(STR_NL);
    LOG("<-- D : "USZ_FMT STR_NL, mm_queue_dequeue(&queue));

    LOG(STR_NL);
    for (usz i = 0; i < mm_queue_length(&queue); ++i) {
        LOG("<-- D : "USZ_FMT STR_NL, mm_queue_dequeue(&queue));
    }

    return OS_EXIT_SUCCESS;
}
