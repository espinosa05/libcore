#ifndef __CORE_M_BUFFER_H__
#define __CORE_M_BUFFER_H__

#define M_BUFFER(...) (struct m_buffer) { __VA_ARGS__ }
#define M_BUFFER_REF(...) &M_BUFFER(__VA_ARGS__)

struct m_buffer {
    void *base;
    usz cursor;
    usz size;
};

struct m_buffer_info {
    void *buffer;
    usz size;
};

enum m_buffer_status_codes {
    M_BUFFER_STATUS_SUCCESS = 0,
    M_BUFFER_STATUS_OUT_OF_MEMORY,
    M_BUFFER_STATUS_OUT_OF_BOUNDS_READ,
    M_BUFFER_STATUS_OUT_OF_BOUNDS_WRITE,
    M_BUFFER_STATUS_OUT_OF_BOUNDS_CURSOR,
};
typedef u32 M_Buffer_Status;

void m_buffer_init(struct m_buffer *buffer, const struct m_buffer_info info);
M_Buffer_Status m_buffer_read(struct m_buffer *buffer, void *dst, usz dst_cap, usz ammount);
M_Buffer_Status m_buffer_write(struct m_buffer *buffer, void *src, usz dst_cap, usz ammount);
M_Buffer_Status m_buffer_set_cursor(struct m_buffer *buffer, usz pos);


#endif
