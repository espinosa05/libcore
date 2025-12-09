#ifndef __CORE_JOB_H__
#define __CORE_JOB_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/os_thread.h>
#include <core/os_lock.h>

struct job_queue_info {
    usz queue_length;
    usz thread_count;
    struct m_arena *arena;
};

struct job_queue {
    struct os_mutex access_mutex;
    struct m_queue  job_info_queue;
    struct m_array  thread_pool;
};

struct job {
    void    (*function) (void *);
    void    *args;
};

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info);
void job_queue_add(struct job_queue *jobs, const struct job *job);
void job_queue_drain(struct job_queue *jobs, struct job *job);

#endif /* __CORE_JOB_H__ */
