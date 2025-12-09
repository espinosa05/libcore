#include <core/queue.h>
#include <core/utils.h>

Queue_Status queue_init(struct queue *queue, const struct queue_info info)
{
    CHECK_NULL(info.arena);
    queue->cap      = info.cap;
    queue->count    = 0;
    queue->base     = m_arena_alloc(info.arena, info.width, info.cap);
    queue->heap     = FALSE;

    return QUEUE_STATUS_SUCCESS;
}

Queue_Status queue_add(struct queue *queue, void *data)
{
    
    return QUEUE_STATUS_SUCCESS;
}

Queue_Status queue_remove(struct queue *queue, void *data)
{
    return QUEUE_STATUS_SUCCESS;
}


