#include <core/job.h>

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info)
{
    void *memory = NULL;
    if (info.arena) {
        memory = m_arena_alloc(info.arena, sizeof(struct job_info), info.count);
    } else {
        memory = m_alloc(sizeof(struct job_info), info.count);
    }

    struct m_array_info job_array_info = {
        .base   = memory,
        .width  = sizeof(struct job_info),
        .cap    = info.count,
    };
    m_array_init_ext(&jobs->job_array, job_array_info);
}

void job_queue_add(struct job_queue *jobs, struct job_info *job_info)
{
    m_array_append(&jobs->job_array, job_info);
}


