#include <core/memory.h>
#include <core/buffer.h>
#include <core/strings.h>
#include <core/log.h>

void _m_arena_init(struct m_arena *arena, const struct m_arena_info info)
{
    arena->mem_avail = info.mem_size;
    if (info.external) {
        arena->buffer = info.buffer;
    } else {
        arena->heap = TRUE;
        arena->buffer = m_alloc(BYTE_SIZE, info.mem_size);
    }

    arena->mem_used = 0;
}

void *_m_arena_alloc(struct m_arena *arena, usz size, usz count)
{
    void *buff = NULL;
    usz request = size * count;

    if (arena->mem_used + request <= arena->mem_avail) {
        buff = U8_PTR(arena->buffer) + arena->mem_used;
        arena->mem_used += request;
    }

    ASSERT(buff, "ALLOCATION FAILED");
    return buff;
}

void _m_arena_clear(struct m_arena *arena)
{
    arena->mem_used = 0;
}

void _m_arena_destroy(struct m_arena *arena)
{
    if (arena->heap)
        m_free(arena->buffer);
}

void _m_arena_save(struct m_arena *arena, usz *save)
{
    *save = arena->mem_used;
}

void _m_arena_restore(struct m_arena *arena, usz save)
{
    arena->mem_used = save;
}

void m_buffer_init(struct m_buffer *buffer, const struct m_buffer_info info)
{
    if (info.external) {
        buffer->base = info.buffer;
    } else {
        buffer->dynamic = TRUE;
        buffer->base = m_alloc(BYTE_SIZE, info.size);
    }

    buffer->cursor = 0;
    buffer->size = info.size;
}

void m_buffer_resize(struct m_buffer *buffer, usz new_size)
{
    ASSERT(buffer->dynamic, "can't resize static buffer!");
    buffer->base = m_realloc(buffer->base, BYTE_SIZE, new_size);
    buffer->size = new_size;
}

M_Buffer_Status m_buffer_write(struct m_buffer *buffer, void *src, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    m_copy(src, buffer->base, ammount);
    buffer->cursor += ammount;

    return M_BUFFER_STATUS_SUCCESS;
}

M_Buffer_Status m_buffer_read(struct m_buffer *buffer, void *dst, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    m_copy(U8_PTR(buffer->base) + buffer->cursor, dst, ammount);
    buffer->cursor += ammount;

    return M_BUFFER_STATUS_SUCCESS;
}

M_Buffer_Status m_buffer_set_cursor(struct m_buffer *buffer, usz pos)
{
    if (pos > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_BOUNDS_CURSOR;

    buffer->cursor = pos;
    return M_BUFFER_STATUS_SUCCESS;
}

void m_buffer_delete(const struct m_buffer *buffer)
{
    m_free(buffer->base);
}

void m_array_init(struct m_array *array, usz width, usz init_size)
{
    array->data     = m_alloc(width, init_size);
    array->width    = width;
    array->count    = 0;
    array->cap      = init_size;
    array->dynamic  = TRUE;
}

void m_array_init_ext(struct m_array *array, const struct m_array_info info)
{
    ASSERT(info.base, "array can't point to NULL!");
    array->data     = info.base;
    array->width    = info.width;
    array->count    = info.count;
    array->cap      = info.cap;
    array->dynamic  = FALSE;
}

void m_array_insert(struct m_array *array, usz index, void *element)
{
    if (index > array->cap) {
        ASSERT_RT(array->dynamic, "array out of memory!\n" M_ARRAY_FMT, M_ARRAY_FMT_ARG(*array));
        array->data = m_realloc(array->data, array->width, index + 1);
    }

    if (index > array->count)
        array->count = index;

    void *dst = m_array_get_addr(array, index);

    m_copy(dst, element, array->width);
}

void m_array_get(const struct m_array *array, usz index, void *element)
{
    /* about the only time we really need to modify the data pointed to by 'm_array_get_addr' */
    void *src = m_array_get_addr(array, index);
    m_copy(element, src, array->width);
}

void *m_array_get_addr(const struct m_array *array, usz index)
{
    ASSERT(index < array->count, "m_array("USZ_FMT") out of bounds read: @ "PTR_FMT" + "USZ_FMT,
                                  index,
                                  array->data,
                                  index * array->width);

    return U8_PTR(array->data) + (array->width + index);
}

void m_array_append(struct m_array *array, void *element)
{
    m_array_insert(array, array->count, element);
    array->count++;
}

void m_array_copy(const struct m_array *src, struct m_array *dst)
{
    dst->data = m_alloc(dst->width, dst->count);
    m_copy(dst->data, src->data, dst->width * dst->count);
}

void m_array_delete(struct m_array array)
{
    if (array.data && array.count)
        m_free(array.data);
}

#define M_STACK_DEFAULT_INIT_CAPACITY 0x20
M_Stack_Status m_stack_init(struct m_stack *stack, const struct m_stack_info info)
{
    ASSERT(info.element_size, "[BUG] "STR_SYM(info.element_size)" should be non-zero for all buffers!");
    stack->cap = info.cap;
    stack->sp = 0;

    if (info.external) {
        ASSERT_RT(info.cap, "[BUG] "STR_SYM(info.cap)" should be non-zero for externally managed buffers!");
        ASSERT_RT(info.buffer, "[BUG] "STR_SYM(info.base)" should point to a buffer of size "USZ_FMT"!", info.cap);
        stack->base = info.buffer;
    } else {
        usz init_cap = info.cap ? info.cap : M_STACK_DEFAULT_INIT_CAPACITY;
        stack->base = m_alloc(info.element_size, init_cap);
    }

    return M_STACK_STATUS_SUCCESS;
}

M_Stack_Status m_stack_push(struct m_stack *stack, const void *element)
{
    if (stack->sp + 1 >= stack->cap)
        return M_STACK_STATUS_EXHAUSTED;

    usz offset = stack->sp * stack->element_size;
    m_copy(U8_PTR(stack->base) + offset, element, stack->element_size);

    if (stack->sp == stack->cap - 1)
        return M_STACK_STATUS_LAST_ELEMENT;

    stack->sp++;

    return M_STACK_STATUS_SUCCESS;
}

M_Stack_Status m_stack_pop(struct m_stack *stack, void *element)
{
    usz offset = stack->sp * stack->element_size;
    m_copy(element, U8_PTR(stack->base) + offset, stack->element_size);

    if (stack->sp == 0)
        return M_STACK_STATUS_EXHAUSTED;

    stack->sp--;

    return M_STACK_STATUS_SUCCESS;
}

M_Stack_Status m_stack_push_array(struct m_stack *stack, const struct m_array array)
{
    if (stack->sp + array.count >= stack->cap)
        return M_STACK_STATUS_EXHAUSTED;

    usz offset = stack->sp * stack->element_size;
    m_copy(U8_PTR(stack->base) + offset, array.data, array.width * array.count);

    if (stack->sp == stack->cap)
        return M_STACK_STATUS_LAST_ELEMENT;

    stack->sp += array.count;

    return M_STACK_STATUS_SUCCESS;
}

M_Stack_Status m_stack_delete(const struct m_stack stack)
{
    m_free(stack.base);
    return M_STACK_STATUS_SUCCESS;
}

static const char *m_stack_status_strs[M_STACK_STATUS_COUNT] = {
    ENUM_STR_ENTRY(M_STACK_STATUS_SUCCESS),
    ENUM_STR_ENTRY(M_STACK_STATUS_LAST_ELEMENT),
    ENUM_STR_ENTRY(M_STACK_STATUS_EXHAUSTED),
    /* error codes */
    ENUM_STR_ENTRY(M_STACK_STATUS_OUT_OF_MEMORY),

    ENUM_STR_ENTRY(M_STACK_STATUS_UNKNOWN),
};

const char *m_stack_get_status_str(usz st)
{
    if (st > M_STACK_STATUS_COUNT)
        st = M_STACK_STATUS_UNKNOWN;

    return m_stack_status_strs[st];
}

