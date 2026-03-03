#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdarg.h>

/* legacy type compatibility */
typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

/* new types */
typedef ptrdiff_t   sz;
typedef sz          ssz;
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

enum                b32_values { FALSE = 0, TRUE = 1 };
typedef u32         b32; /* boolean type */

typedef const char  *cstr; /* looks pretty while casting */

typedef intmax_t    smax;
typedef uintmax_t   umax;

#define U8_MIN      0
#define U16_MIN     0
#define U32_MIN     0
#define U64_MIN     0
#define USZ_MIN     0
#define UMAX_MIN    0

#define U8_MAX      255U
#define U16_MAX     65535U
#define U32_MAX     4294967295UL
#define U64_MAX     18446744073709551615ULL
#define USZ_MAX     ((1<<sizeof(usz)*8))
#define UMAX_MAX    UINTMAX_MAX

#define S8_MAX      (((1<<sizeof(s8)*8)/2)-1)
#define S16_MAX     (((1<<sizeof(s16)*8)/2)-1)
#define S32_MAX     (((1<<sizeof(s32)*8)/2)-1)
#define S64_MAX     (((1<<sizeof(s64)*8)/2)-1)
#define SSZ_MAX     (((1<<sizeof(ssz)*8)/2)-1)
#define SMAX_MAX    INTMAX_MAX

#define B32_FMT         "%s"
#define B32_FMT_ARG(b)  (b) ? "TRUE" : "FALSE"

#define SZ_FMT      "%"PRIi64   /* weird and hardcoded */
#define SSZ_FMT     SZ_FMT
#define USZ_FMT     "%"PRIu64   /* applies to this aswell */

#define PTR_FMT     "%p"

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
#define SSZ_N_FMT(n)    "%"#n PRIi64   /* weird and hardcoded */
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

#define TYPE_ARRAY_ELEMENT(a, i, ...) ((__VA_ARGS__ *)(a)) [(i)]

#define SZ_ARRAY_ELEMENT(a, i)      TYPE_ARRAY_ELEMENT(a, i, sz)
#define SSZ_ARRAY_ELEMENT(a, i)     TYPE_ARRAY_ELEMENT(a, i, ssz)
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

#define CHAR_FMT    "%c"

#define STR_FMT     "%s"
#define STR_QUOT(s) "\"" s "\""

#define F32(n)  ((f32)(n))
#define F64(n)  ((f64)(n))

#define S8(n)   ((s8)(n))
#define S16(n)  ((s16)(n))
#define S32(n)  ((s32)(n))
#define S64(n)  ((s64)(n))

#define U8(n)   ((u8)(n))
#define U16(n)  ((u16)(n))
#define U32(n)  ((u32)(n))
#define U64(n)  ((u64)(n))

#define SSZ(n)  ((ssz)(n))
#define USZ(n)  ((usz)(n))
#define PTR(p)  ((void *)(p))

#define S8_PTR(ptr)     ((s8 *)(ptr))
#define S16_PTR(ptr)    ((s16 *)(ptr))
#define S32_PTR(ptr)    ((s32 *)(ptr))
#define S64_PTR(ptr)    ((s64 *)(ptr))

#define U8_PTR(ptr)     ((u8 *)(ptr))
#define U16_PTR(ptr)    ((u16 *)(ptr))
#define U32_PTR(ptr)    ((u32 *)(ptr))
#define U64_PTR(ptr)    ((u64 *)(ptr))
#define USZ_PTR(ptr)    ((usz *)(ptr))

#define S8_REF(s)   &(s8) {(s)}
#define S16_REF(s)  &(s16) {(s)}
#define S32_REF(s)  &(s32) {(s)}
#define S64_REF(s)  &(s64) {(s)}
#define SSZ_REF(s)  &(ssz) {(s)}

#define U8_REF(u)   &(u8) {(u)}
#define U16_REF(u)  &(u16) {(u)}
#define U32_REF(u)  &(u32) {(u)}
#define U64_REF(u)  &(u6) {(u)}
#define USZ_REF(u)  &(usz) {(u)}

#define DECL_FUNC_PTR(ret, name, ...)  ret (*name) (__VA_ARGS__)
/* used for type casting */
#define FUNC_PTR_TYPE(ret, ...) ret (*) (__VA_ARGS__)

#endif /* __TYPES_H__ */
