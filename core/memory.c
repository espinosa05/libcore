#include <core/memory.h>

void m_arena_init(struct m_arena *arena, const struct m_arena_info info)
{
    arena->mem_avail = info.mem_size;
    if (info.external) {
        arena->buffer = info.buffer;
    } else {
        arena->heap = TRUE;
        arena->buffer = m_alloc(BYTE_SIZE, info.mem_size);
    }
}

void *m_arena_alloc(struct m_arena *arena, usz size, usz count)
{
    void *buff = (u8 *)arena->buffer + arena->mem_used;
    arena->mem_used += size * count;

    if (arena->mem_used > arena->mem_avail)
        return NULL;

    return buff;
}

void m_arena_reset(struct m_arena *arena)
{
    arena->mem_used = 0;
}

void m_arena_destroy(struct m_arena *arena)
{
    if (arena->heap)
        m_free(arena->buffer);
}

void m_buffer_init(struct m_buffer *buffer, const struct m_buffer_info info)
{
    buffer->base = info.buffer;
    buffer->size = info.size;
}

M_Buffer_Status m_buffer_write(struct m_buffer *buffer, void *src, usz dst_cap, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    if (dst_cap < ammount)
        return M_BUFFER_STATUS_OUT_OF_BOUNDS_READ;

    m_copy(src, buffer->base, ammount);
    return M_BUFFER_STATUS_SUCCESS;
}

M_Buffer_Status m_buffer_read(struct m_buffer *buffer, void *dst, usz dst_cap, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    if (dst_cap < ammount)
        return M_BUFFER_STATUS_OUT_OF_BOUNDS_WRITE;

    m_copy((u8 *)buffer->base + buffer->cursor, dst, ammount);
    buffer->cursor += ammount;

    return M_BUFFER_STATUS_SUCCESS;
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
    array->data     = info.base;
    array->width    = info.width;
    array->count    = 0;
    array->cap      = info.cap;
    array->dynamic  = FALSE;
}

void m_array_insert(struct m_array *array, usz index, void *element)
{
    if (index > array->cap) {
        ASSERT_RT(array->dynamic, "static array ran out of memory:\n"
                                  STR_SYM(array->cap)": %d\n"
                                  STR_SYM(index)": %d\n",
                                  array->cap, index);

        array->data = m_realloc(array->data, array->width, index + 1);
    }

    if (index > array->count)
        array->count = index;

    m_copy((u8 *)array->data + (index * array->width), element, array->width);
}

void m_array_get(const struct m_array *array, usz index, void *element)
{
    m_copy(element, (u8 *)array->data + (index * array->width), array->width);
}

void m_array_append(struct m_array *array, void *element)
{
    m_array_insert(array, array->count, element);
    array->count++;
}

void m_array_copy(const struct m_array *src, struct m_array *dst)
{
    *dst = *src;
    dst->data = m_alloc(dst->width, dst->count);
    m_copy(dst->data, src->data, src->width * src->count);
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
        ASSERT(info.cap, "[BUG] "STR_SYM(info.cap)" should be non-zero for externally managed buffers!");
        ASSERT(info.buffer, "[BUG] "STR_SYM(info.base)" should point to a buffer of size %d!", info.cap);
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
    m_copy((u8 *)stack->base + offset, element, stack->element_size);

    if (stack->sp == stack->cap - 1)
        return M_STACK_STATUS_LAST_ELEMENT;

    stack->sp++;

    return M_STACK_STATUS_SUCCESS;
}

M_Stack_Status m_stack_pop(struct m_stack *stack, void *element)
{
    usz offset = stack->sp * stack->element_size;
    m_copy(element, (u8 *)stack->base + offset, stack->element_size);

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
    m_copy((u8 *)stack->base + offset, array.data, array.width * array.count);

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
