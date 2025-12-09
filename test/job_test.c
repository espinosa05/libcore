#include <core/types.h>
#include <core/memory.h>
#include <core/job.h>
#include <core/log.h>

#include <string.h>


#define JOB_FUNC(j) ((void (*) (void *))j)
#define JOB_ARGS(...) ((void *)__VA_ARGS__)

struct os_mutex print_lock = {0};

#define MAX_JOBS 10

struct test_job0_args {
    usz     number;
    char    *string;
};

void test_job0(struct test_job0_args *args)
{
    os_mutex_lock(&print_lock);
    INFO_LOG("test_job0: "STR_SYM(args->number)": %d", args->number);
    INFO_LOG("test_job0: "STR_SYM(args->string)": %s", args->string);
    os_mutex_unlock(&print_lock);
}

struct test_job1_args {
    usz *array;
    usz count;
};

void test_job1(struct test_job1_args *args)
{
    for (usz i = 0; i < args->count; ++i) {
        os_mutex_lock(&print_lock);
        INFO_LOG("test_job1: "STR_SYM(args->array[i])": %d", args->array[i]);
        os_mutex_unlock(&print_lock);
    }
}


struct test_job2_args {
    usz number;
};

void test_job2(struct test_job2_args *args) {
    os_mutex_lock(&print_lock);
    if (args->number == 0) {
        WARN_LOG("test_job2: "STR_SYM(args->number)": is zero!");
    } else {
        INFO_LOG("test_job2: "STR_SYM(args->number)": %d", args->number);
    }
    os_mutex_unlock(&print_lock);
}

int main(int argc, char **argv)
{
    os_mutex_init(&print_lock);

    struct m_arena arena = {0};
    struct m_arena_info arena_info = {
        .buffer     = (u8 [20*KB_SIZE]) {0},
        .mem_size   = KB_SIZE,
        .external   = TRUE,
    };
    m_arena_init(&arena, arena_info);

    struct job_queue jobs = {0};
    struct job_queue_info jobs_info = {
        .count  = MAX_JOBS,
        .arena  = &arena,
    };
    job_queue_init(&jobs, jobs_info);

#define SIZE 3
    struct job_info job_data[] = {
        { JOB_FUNC(test_job0), JOB_ARGS(&(struct test_job0_args) { 32, "Hello World!", })},
        { JOB_FUNC(test_job1), JOB_ARGS(&(struct test_job1_args) { (usz [SIZE]) {3, 4, 43}, SIZE, })},
        { JOB_FUNC(test_job2), JOB_ARGS(&(struct test_job2_args) { 0 })}
    };

    for (usz i = 0; i < ARRAY_SIZE(job_data); ++i) {
        job_queue_add(&jobs, &job_data[i]);
    }

    return 0;
}
