#include <core/strings.h>
#include <core/memory.h>

/* static function declaration start */
static b32 is_delim(const char c, const char *delim);
/* static function declaration end */

void str_builder_init(struct str_builder *sb, usz init_cap)
{
    sb->count       = 0;
    sb->cap         = init_cap;
    sb->elements    = m_alloc(sizeof(*sb->elements), sb->cap);
}

void str_builder_init_ext(struct str_builder *sb, const struct m_buffer buff)
{
    sb->count       = 0;
    sb->cap         = buff.size;
    sb->elements    = buff.base;
}

Str_Builder_Status str_builder_append(struct str_builder *sb, const char *cstr)
{
    ASSERT(sb->count + 1 <= sb->cap, "string builder corrupted!");

    sb->count++;

    if (sb->count == sb->cap) {
        if (sb->external) {
            return STR_BUILDER_STATUS_EXHAUSTED;
        }
        sb->cap++;
        sb->elements = m_realloc(sb->elements, sizeof(*sb->elements), sb->cap);
    }
    sb->elements[sb->count - 1] = (char *)cstr;

    return STR_BUILDER_STATUS_SUCCESS;
}

void str_builder_to_cstr(const struct str_builder *sb, char *dst, usz cap)
{
    for (usz i = 0; i < sb->count; ++i) {
        cstr_concat(dst, sb->elements[i], cap);
        cap -= cstr_length(sb->elements[i]);
        ASSERT(cap > 0, "can't copy str_builder element "STR_QUOT_LIT(STR_FMT)": no space remaining",
                        sb->elements[i]);
    }
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

void str_builder_to_cstr_ar(const struct str_builder *sb, char **dst, struct m_arena *arena)
{
    char *base; /* stores the base address of the buffer */
    char *cursor; /* keeps track of the elements */
    usz element_length;
    usz buff_length;

    /* claim arena mutex for consecutive allocations */
    m_arena_claim(arena);

    /* create first element */
    element_length = cstr_length(sb->elements[0]);
    base = m_arena_alloc_claimed(arena, sizeof(*base), element_length + NULL_TERM_SIZE);
    cursor = base;
    buff_length = element_length;
    cstr_copy(cursor, sb->elements[0], element_length);
    NULL_TERM_BUFF(cursor, element_length);

    for (usz i = 1; i < sb->count; ++i) {
        /* reallocate the buffer */
        base = m_arena_alloc_claimed(arena, sizeof(*sb->elements), buff_length);

        cstr_copy(cursor, sb->elements[i], element_length);
        NULL_TERM_BUFF(cursor, element_length);
        cursor += element_length;
        buff_length += element_length;
    }

    m_arena_release(arena);

    *dst = base;
}

void str_builder_delete(const struct str_builder sb)
{
    m_free(sb.elements);
}

char cstr_char_at_backwards(char *str, usz pos)
{
    return str[(cstr_length(str) - 1) - pos];
}

void cstr_format_alloc_variadic(char **buffer, const char *fmt, va_list args, usz *length)
{
    usz alloc_size = 0;
    alloc_size = vasprintf(buffer, fmt, args);

    if (length)
        *length = alloc_size;
}

void cstr_format_ar(char **buffer, struct m_arena *arena, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    cstr_format_variadic_ar(buffer, arena, fmt, args);
}

void cstr_format_variadic_ar(char **buffer, struct m_arena *arena, const char *fmt, va_list args)
{
    usz alloc_size = 0;

    alloc_size = vsnprintf(NULL, 0, fmt, args);
    *buffer = m_arena_alloc(arena, alloc_size, 1);
    vsnprintf(*buffer, alloc_size, fmt, args);
}

void cstr_token_ar(char **buffer, const char *str, const char *delim, const char **tok_ptr, struct m_arena *arena)
{
    CHECK_NULL(buffer);
    CHECK_NULL(delim);
    CHECK_NULL(tok_ptr);
    CHECK_NULL(arena);

    const char *remain = NULL;
    usz count = 0;

    if (!tok_ptr) {
        remain = str;
    } else {
        remain = *tok_ptr + 1;
    }

    for (count = 0; count < cstr_length(remain); ++count) {
        char c = remain[count];
        if (is_delim(c, delim))
            break;
    }

    *buffer = m_arena_alloc(arena, BYTE_SIZE, count);
    cstr_copy(*buffer, remain, count);

    *tok_ptr = remain + count;
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

b32 cstr_contains(const char *str, const char *sub)
{
    for (usz i = 0; i < cstr_length(str); ++i) {
        if (cstr_compare(str + i, sub))
            return TRUE;
    }

    return FALSE;
}

b32 cstr_contains_char(const char *str, const char c)
{
    for (usz i = 0; i < cstr_length(str); ++i) {
        if (c == str[i])
            return TRUE;
    }

    return FALSE;
}

static b32 is_delim(const char c, const char *const delim)
{
    return cstr_contains_char(delim, c);
}

