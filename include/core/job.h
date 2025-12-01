#ifndef __CORE_JOB_H__
#define __CORE_JOB_H__

#include <core/types.h>
#include <core/memory.h>
#include <core/os_thread.h>

struct job_queue_info {
    usz count;
    /* not required */
    struct m_arena *arena;
};

struct job_queue {
    struct m_array job_array;
};

struct job_info {
    void                (*function) (void *);
    void                *args;
};

void job_queue_init(struct job_queue *jobs, const struct job_queue_info info);
void job_queue_add(struct job_queue *jobs, struct job_info *job_info);

#endif /* __CORE_JOB_H__ */
