#ifndef __CORE_BM_FONTS_H__
#define __CORE_BM_FONTS_H__

#include <core/memory_arena.h>
#include <core/types.h>

struct bm_font_spec {
    u32 count;
    u32 width;
    u32 height;
    void *bitmap;
};

#define BM_FONT_MAGIC_NUMBER 0xFAB0FAB0
/*
 * Header size
 * MAGIC_NUMBER + 4
 * count        + 4
 * width        + 4
 * height       + 4
 * TOTAL        = 28
 * */
#define BM_FONT_HEADER_SIZE (sizeof(u32) + sizeof(u32) + sizeof(u32) + sizeof(u32))

typedef usz BM_Font_Status;
enum bm_font_status_codes {
    BM_FONT_STATUS_SUCCESS = 0,
    BM_FONT_STATUS_NOT_A_FONT_FILE,
    BM_FONT_STATUS_FONT_FILE_MISSING,
    BM_FONT_STATUS_MALFORMED_FONT_FILE,

    BM_FONT_STATUS_COUNT,
#define BM_FONT_STATUS_UNKNOWN BM_FONT_STATUS_COUNT
};

BM_Font_Status bm_font_read_font_file_ar(const char *file_path, struct bm_font_spec *out, struct m_arena *arena);
char *bm_font_status_str(usz st);

#endif /* __CORE_BM_FONTS_H__ */
