#ifndef __CORE_STRINGS_H__
#define __CORE_STRINGS_H__

#include <core/memory.h>
#include <core/buffer.h>
#include <core/types.h>
#include <core/utils.h>
#include <core/cstd.h>
#include <core/variadic.h>

struct string_view {
    char *start;
    char *end;
};

struct str_builder {
    char    **elements;
    usz     count;
    usz     cap;
    b32     external;
};
enum str_builder_status_codes {
    STR_BUILDER_STATUS_SUCCESS = 0,
    STR_BUILDER_STATUS_EXHAUSTED,
};
typedef usz Str_Builder_Status;

void str_builder_init(struct str_builder *sb, usz init_cap);
Str_Builder_Status str_builder_append(struct str_builder *sb, char *str);
void str_builder_to_cstr_alloc(const struct str_builder *sb, char **dst);
void str_builder_delete(const struct str_builder sb);
void str_builder_init_ext(struct str_builder *sb, const struct m_buffer buff);

#define STR_NL  "\n"
#define STR_TAB "\t"

#define STR_SYM(s) #s
#define STR_SYM_FMT(s) #s": %s"
#define STR_SYM_ARG(s) STR_SYM(s)
#define STR_SYM_MSG(s) STR_SYM_FMT(s) STR_SYM_ARG(s)
#define STR_SYM_QUOT(s) "\""STR_SYM(s)"\""

#define NULL_TERM_SIZE 1
#define NULL_TERM_BUFF(s, n) (s)[n] = '\0'
#define CONST_STRLEN(str)   (sizeof(str)/sizeof(*str)-NULL_TERM_SIZE)

#define cstr_length strlen
#define cstr_compare(s1, s2) (0 == strcmp(s1, s2))
#define cstr_concat strncat
#define cstr_copy strncpy

char *cstr_format(char *buffer, usz size, const char *fmt, ...);
char *cstr_format_variadic(char *buffer, usz size, const char *fmt, va_list args, usz *final_length);
void cstr_format_alloc(char **buffer, const char *fmt, ...);
void cstr_format_alloc_variadic(char **buffer, const char *fmt, va_list args, usz *length);

#endif /* __CORE_STRINGS_H__ */
