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

M_Buffer_Status m_buffer_read(struct m_buffer *buffer, void *src, usz dst_cap, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    if (dst_cap < ammount)
        return M_BUFFER_STATUS_OUT_OF_BOUNDS_READ;

    m_copy(src, buffer->base, ammount);
    return M_BUFFER_STATUS_SUCCESS;
}

M_Buffer_Status m_buffer_write(struct m_buffer *buffer, void *dst, usz dst_cap, usz ammount)
{
    if (buffer->cursor + ammount > buffer->size)
        return M_BUFFER_STATUS_OUT_OF_MEMORY;

    if (dst_cap < ammount)
        return M_BUFFER_STATUS_OUT_OF_BOUNDS_WRITE;

    m_copy(buffer->base, dst, ammount);
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
    array->count    = info.count;
    array->cap      = info.cap;
    array->dynamic  = FALSE;
}

void m_array_insert(struct m_array *array, usz index, void *element)
{
    if (index > array->cap) {
        ASSERT_RT(array->dynamic, "static array ran out of memory");
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
    m_copy(dst->data, src->data, src->width*src->count);
}

void m_array_delete(struct m_array array)
{
    if (array.data && array.count)
        m_free(array.data);
}

void init_persistent_arena(void)
{

}

