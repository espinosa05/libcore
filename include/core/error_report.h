#ifndef __CORE_ERROR_REPORTING_H__
#define __CORE_ERROR_REPORTING_H__

#include <core/types.h>
#include <core/utils.h>

struct error_report {
    usz     r_size;
    char    *r_string;
};

#define EMPTY_ERROR_REPORT(er) (!(er).r_string && !(er).r_size)

void er_append_report(struct error_report *er, char *report_fmt, ...);
void er_clear_report(struct error_report *er);

#endif /* __CORE_ERROR_REPORTING_H__ */
