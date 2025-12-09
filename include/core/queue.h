#ifndef __CORE_QUEUE_H__
#define __CORE_QUEUE_H__

#include <core/types.h>
#include <core/memory.h>

enum queue_status_codes {
    QUEUE_STATUS_SUCCESS = 0,
    QUEUE_STATUS_LAST_ELEMENT,
    QUEUE_STATUS_EXHAUSTED,
    QUEUE_STATUS_FULL,
};
typedef usz Queue_Status;

struct queue_info {
    usz             width;
    usz             cap;
    struct m_arena  *arena;
};

struct queue {
    struct  ll_double data;
};
#define QUEUE(...) (struct queue) {__VA_ARGS___}
#define QUEUE_REF(...) &QUEUE(__VA_ARGS__)

const char *queue_get_status_str(usz st);

Queue_Status queue_init(struct queue *queue, const struct queue_info info);
Queue_Status queue_add(struct queue *queue, void *data);
Queue_Status queue_remove(struct queue *queue, void *data);

#endif /* __CORE_QUEUE_H__ */
