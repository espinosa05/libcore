#include <core/bm_fonts.h>
#include <core/types.h>
#include <core/strings.h>
#include <core/os_file.h>
#include <core/utils.h>
#include <core/memory.h>

BM_Font_Status bm_font_read_font_file_ar(const char *file_path, struct bm_font_spec *out, struct m_arena *arena)
{
    struct os_file font_file = {0};
    struct os_file_info font_file_info = {0};
    font_file_info.path = file_path;
    font_file_info.perm = FPERM_READ;
    usz st = os_file_open(&font_file, font_file_info);

    if (OS_FILE_STATUS_NO_SUCH_FILE == st)
        return BM_FONT_STATUS_FONT_FILE_MISSING;

    if (OS_FILE_STATUS_PERMISSION_DENIED == st)
        return BM_FONT_STATUS_MALFORMED_FONT_FILE;

    INFO_LOG("Bitmap Font File Information:");

    /* read the header */
    u32 magic = 0;
    usz font_file_size = os_file_get_size(&font_file);
    LOG("size:"STR_TAB USZ_FMT STR_NL, font_file_size);
    if (BM_FONT_HEADER_SIZE > font_file_size)
        return BM_FONT_STATUS_NOT_A_FONT_FILE;

    INFO_LOG("HEADER:");
    os_file_read_le32(&font_file, &magic);
    LOG("magic:"STR_TAB U32_X_FMT STR_NL, magic);
    if (magic != BM_FONT_MAGIC_NUMBER)
        return BM_FONT_STATUS_NOT_A_FONT_FILE;

    struct bm_font_spec spec = {0};
    os_file_read_le32(&font_file, &spec.count);
    os_file_read_le32(&font_file, &spec.width);
    os_file_read_le32(&font_file, &spec.height);

    LOG("count:"STR_TAB U32_FMT STR_NL, spec.count);
    LOG("width:"STR_TAB U32_FMT STR_NL, spec.width);
    LOG("height:"STR_TAB U32_FMT STR_NL, spec.height);
    /* the bitmap width and height of each cell must be rounded to
     * the next byte
     * */
    usz font_bitmap_size = spec.count * align_up(spec.width, 8) * align_up(spec.height, 8);
    font_bitmap_size /= BITS_IN_BYTE;
    if (font_bitmap_size < font_file_size - BM_FONT_HEADER_SIZE)
        return BM_FONT_STATUS_MALFORMED_FONT_FILE;

    /* finally, read the font bitmap */
    spec.bitmap = m_arena_alloc(arena, BYTE_SIZE, font_bitmap_size);
    os_file_read(&font_file, spec.bitmap, font_bitmap_size);

    *out = spec;

    os_file_close(&font_file);
    return BM_FONT_STATUS_SUCCESS;
}

static char *g_bm_font_status_strs[] = {
    ENUM_STR_ENTRY(BM_FONT_STATUS_SUCCESS),
    ENUM_STR_ENTRY(BM_FONT_STATUS_NOT_A_FONT_FILE),
    ENUM_STR_ENTRY(BM_FONT_STATUS_FONT_FILE_MISSING),
    ENUM_STR_ENTRY(BM_FONT_STATUS_MALFORMED_FONT_FILE),
    ENUM_STR_ENTRY(BM_FONT_STATUS_UNKNOWN),
};

char *bm_font_status_str(usz st)
{
    if (st > BM_FONT_STATUS_COUNT)
        st = BM_FONT_STATUS_UNKNOWN;

    return g_bm_font_status_strs[st];
}
