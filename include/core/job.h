#ifndef __CORE_JOB_H__
#define __CORE_JOB_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/queue.h>
#include <core/os_thread.h>
#include <core/os_lock.h>

struct job_queue_info {
    usz queue_length;
    usz thread_count;
    struct m_arena *arena;
};

struct job_queue {
    struct os_mutex access_mutex;
    struct queue  job_queue;
    struct m_array thread_pool;
    b32 drain;
};

struct job {
    void    (*func) (void *);
    void    *args;
};

#define JOB_FUNC(func) (void (*) (void *)) func
#define JOB_ARGS(args) (void *)args

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info);
void job_queue_add(struct job_queue *jobs, const struct job *job);
void job_queue_drain(struct job_queue *jobs);

#endif /* __CORE_JOB_H__ */
