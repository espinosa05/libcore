#include <core/job.h>
#include <core/queue.h>
#include <core/utils.h>

/* static function declaration start */
static void job_queue_worker_dispatch(struct job_queue *jobs);
/* static function declaration end */

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info)
{
    CHECK_NULL(info.arena);

    jobs->drain = FALSE;

    os_mutex_init(&jobs->access_mutex);
    struct m_array_info thread_pool_info = {
        .base   = m_arena_alloc(info.arena, sizeof(struct os_thread), info.thread_count),
        .width  = sizeof(struct os_thread),
        .cap    = info.thread_count,
    };
    m_array_init_ext(&jobs->thread_pool, thread_pool_info);

    struct queue_info job_queue_info = {
        .arena  = info.arena,
        .width  = sizeof(struct job),
        .cap    = info.queue_length,
    };
    queue_init(&jobs->job_queue, job_queue_info);
    struct job job_queue_worker = {
        .func = JOB_FUNC(job_queue_worker_dispatch),
        .args = JOB_ARGS(&jobs->job_queue),
    };
    for (usz i = 0; i < jobs->thread_pool.count; ++i) {
        os_thread_spawn(m_array_get_addr(&jobs->thread_pool, i),
                        job_queue_worker.func, job_queue_worker.args);
    }
}

void job_queue_add(struct job_queue *jobs, const struct job *job)
{
    os_mutex_lock(&jobs->access_mutex);
    queue_add(&jobs->job_queue, (void *)job);
    os_mutex_unlock(&jobs->access_mutex);
}

void job_queue_drain(struct job_queue *jobs)
{
    os_mutex_lock(&jobs->access_mutex);
    jobs->drain = TRUE;
    os_mutex_unlock(&jobs->access_mutex);
}

static void job_queue_worker_dispatch(struct job_queue *jobs)
{
    UNUSED(jobs);
}
