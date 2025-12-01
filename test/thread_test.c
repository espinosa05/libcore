#include <core/job.h>
#include <core/cpu.h>
#include <core/memory.h>

struct test_job_args {
    char *string_0;
    char *string_1;
    usz return_value;
};

void test_job(struct test_job_args *args)
{
    if (string_0 == string_1) {
        args->return_value = 1;
        return;
    }

    if (cstr_cmp(string_0, string_1)) {
        args->return_value = 1;
    }

    args->return_value = 0;
}

#define BUFF_SIZE KB_SIZE
int main(int argc, char **argv)
{
    struct m_arena job_arena = {0};
    struct m_arena_info job_arena_info = {
        .buffer     = (u8 [BUFF_SIZE]) {0},
        .mem_size   = BUFF_SIZE,
        .external   = TRUE,
    };

    struct job_queue job = {0};
    struct job_queue_info job_info = {
        .count = cpu_get_logical_count(),
        .arena = &job_arena,
    };
    job_queue_init(&job, job_info);

    return 0;
}
