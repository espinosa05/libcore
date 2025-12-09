#include <core/job.h>
#include <core/queue.h>
#include <core/utils.h>

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info)
{
    CHECK_NULL(info.arena);

    os_mutex_init(&jobs->access_mutex);
    struct m_array_info thread_pool_info = {
        .base   = m_arena_alloc(info.arena, sizeof(struct os_thread), info.thread_count),
        .width  = sizeof(struct os_thread),
        .cap    = info.thread_count,
    };
    m_array_init_ext(&jobs->thread_pool, thread_pool_info);

    os_mutex_lock(&jobs->access_mutex);
    struct queue_info job_queue_info = {
        .arena  = info.arena,
        .width  = sizeof(struct job),
        .cap    = info.queue_length,
    };
    queue_init(&jobs->job_queue, job_queue_info);
    os_mutex_unlock(&jobs->access_mutex);
}

void job_queue_add(struct job_queue *jobs, const struct job *job)
{
    os_mutex_lock(&jobs->access_mutex);
    queue_add(&jobs->job_queue, job);
    os_mutex_unlock(&jobs->access_mutex);
}

void job_queue_drain(struct job_queue *jobs, struct job *job)
{
}

