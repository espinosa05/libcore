#include <core/strings.h>
#include <core/memory.h>

void str_builder_init(struct str_builder *sb, usz init_cap)
{
    sb->count       = 0;
    sb->cap         = init_cap;
    sb->elements    = m_alloc(sizeof(*sb->elements), sb->cap);
}

Str_Builder_Status str_builder_append(struct str_builder *sb, char *cstr)
{
    ASSERT(sb->count + 1 <= sb->cap, "string builder corrupted!");

    if (sb->count + 1 == sb->cap) {
        if (sb->external) {
            return STR_BUILDER_STATUS_EXHAUSTED;
        }
        sb->cap++;
        sb->elements = m_realloc(sb->elements, sizeof(*sb->elements), sb->cap);
    }
    sb->elements[sb->count] = cstr;
    sb->count++;

    return STR_BUILDER_STATUS_SUCCESS;
}

void str_builder_to_cstr_alloc(const struct str_builder *sb, char **dst)
{
    char *base; /* stores the base address of the buffer */
    char *cursor; /* keeps track of the elements */
    usz element_length;
    usz buff_length;

    /* create first element */
    element_length = cstr_length(sb->elements[0]);
    base = m_alloc(sizeof(*base), element_length + NULL_TERM_SIZE);
    cursor = base;
    buff_length = element_length;
    cstr_copy(cursor, sb->elements[0], element_length);
    NULL_TERM_BUFF(cursor, element_length);

    for (usz i = 1; i < sb->count; ++i) {
        /* reallocate the buffer */
        base = m_realloc(base, sizeof(*sb->elements), buff_length);
        cstr_copy(cursor, sb->elements[i], element_length);
        NULL_TERM_BUFF(cursor, element_length);
        cursor += element_length;
        buff_length += element_length;
    }

    *dst = base;
}

void str_builder_delete(const struct str_builder sb)
{
    m_free(sb.elements);
}

void cstr_format_alloc_variadic(char **buffer, const char *fmt, va_list args, usz *length)
{
    usz alloc_size = 0;
    alloc_size = vasprintf(buffer, fmt, args);

    if (length)
        *length = alloc_size;
}

void cstr_format_alloc(char **buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    cstr_format_alloc_variadic(buffer, fmt, args, NULL);
}

char *cstr_format_variadic(char *buffer, usz size, const char *fmt, va_list args, usz *final_length)
{
    usz bytes_written = vsnprintf(buffer, size, fmt, args);
    if (final_length)
        *final_length = bytes_written;

    return buffer;
}

char *cstr_format(char *buffer, usz size, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    return cstr_format_variadic(buffer, size, fmt, args, NULL);
}



