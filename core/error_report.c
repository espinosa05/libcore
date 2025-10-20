#include <core/error_report.h>
#include <core/strings.h>
#include <core/variadic.h>
#include <core/memory.h>

void er_clear_report(struct error_report *er)
{
    m_free(er->r_string);
    m_zero(er, sizeof(*er));
}

/* very simple string builder, might extract this to it's own definition later (which means never) */
void er_append_report(struct error_report *er, char *report_fmt, ...)
{
    va_list ap;
    char *report = NULL;

    va_start(ap, report_fmt);
    usz report_len = vasprintf(&report, report_fmt, ap);

    if (!er->r_string) {
        er->r_size = report_len + NULL_TERM_SIZE;
        er->r_string = m_alloc(BYTE_SIZE, er->r_size);
    } else {
        er->r_size += report_len + NULL_TERM_SIZE;
        er->r_string = m_realloc(er->r_string, BYTE_SIZE, er->r_size);
    }

    cstr_concat(er->r_string, report, report_len);
    NULL_TERM_BUFF(er->r_string, er->r_size);
    m_free(report);
}

