#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdarg.h>

typedef ptrdiff_t   sz;
typedef size_t      usz;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef float       f32;
typedef double      f64;

enum                b32Values { FALSE = 0, TRUE = 1 };
typedef uint32_t    b32; /* boolean type */

typedef const char  *cstr_t;    /* looks pretty while casting */

#define U8_MIN      0
#define U16_MIN     0
#define U32_MIN     0
#define U64_MIN     0

#define U8_MAX      255U
#define U16_MAX     65535U
#define U32_MAX     4294967295UL
#define U64_MAX     18446744073709551615ULL

#define S8_MAX      (((1<<8)/2)-1)
#define S16_MAX     (((1<<16)/2)-1)
#define S32_MAX     (((1<<32)/2)-1)
#define S64_MAX     (((1<<64)/2)-1)

#define SZ_FMT      "%"PRIi64   /* weird and hardcoded */
#define USZ_FMT     "%"PRIu64   /* applies to this aswell */

#define PTR_FMT     "%"PRIuPTR

#define U8_FMT      "%"PRIu8
#define U16_FMT     "%"PRIu16
#define U32_FMT     "%"PRIu32
#define U64_FMT     "%"PRIu64

#define S8_FMT      "%"PRIi8
#define S16_FMT     "%"PRIi16
#define S32_FMT     "%"PRIi32
#define S64_FMT     "%"PRIi64

#define U8_X_FMT    "%02"PRIx8
#define U16_X_FMT   "%04"PRIx16
#define U32_X_FMT   "%08"PRIx32
#define U64_X_FMT   "%016"PRIx64
#define USZ_X_FMT   "%llx" /* special case without leading 0 */

#define SZ_N_FMT(n)     "%"#n PRIi64   /* weird and hardcoded */
#define USZ_N_FMT(n)    "%"#n PRIu64   /* applies to this aswell */

#define PTR_N_FMT(n)    "%"#n PRIuPTR

#define U8_N_FMT(n)     "%"#n PRIu8
#define U16_N_FMT(n)    "%"#n PRIu16
#define U32_N_FMT(n)    "%"#n PRIu32
#define U64_N_FMT(n)    "%"#n PRIu64

#define S8_N_FMT(n)     "%"#n PRIi8
#define S16_N_FMT(n)    "%"#n PRIi16
#define S32_N_FMT(n)    "%"#n PRIi32
#define S64_N_FMT(n)    "%"#n PRIi64

#define TYPE_ARRAY_ELEMENT(a, i, ...) ((__VA_ARGS__)(a)[(i)])

#define SZ_ARRAY_ELEMENT(a, i)      TYPE_ARRAY_ELEMENT(a, i, sz)
#define USZ_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, usz)

#define S8_ARRAY_ELEMENT(a, i)      TYPE_ARRAY_ELEMENT(a, i, s8)
#define S16_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, s16)
#define S32_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, s32)
#define S64_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, s64)

#define U8_ARRAY_ELEMENT(a, i)      TYPE_ARRAY_ELEMENT(a, i, u8)
#define U16_ARRAY_ELEMENT(a, i)     TYPE_ARARY_ELEMENT(a, i, u16)
#define U32_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, u32)
#define U64_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, u64)

#define STR_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, cstr_t)

#define STR_FMT     "%s"

#endif /* __TYPES_H__ */
