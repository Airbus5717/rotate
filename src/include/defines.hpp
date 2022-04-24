#if !defined(ROTATE_DEFINE)
#define ROTATE_DEFINE

// TODO: remove C++ stdlib
#include <vector>

// c stdlib
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace rotate
{
#define VERSION "0.0.1"
// clang-format off
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define U8_MAX  UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX

#define S8_MAX  INT8_MAX
#define S16_MAX INT16_MAX
#define S32_MAX INT32_MAX
#define S64_MAX INT64_MAX

typedef unsigned long int usize;
typedef signed   long int isize;

#define ASSERT(expr, msg) \
    if (!(expr))                                                                            \
    {                                                                                              \
        fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__, __LINE__,   \
                RESET);                                                                            \
        exit(1);                                                                                   \
    }

#define ASSERT_CMP(expr1, expr2, msg)                                                                  \
    if (expr1 != expr2)                                                                            \
    {                                                                                              \
        fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__, __LINE__,   \
                RESET);                                                                            \
        exit(1);                                                                                   \
    }

#define ASSERT_NULL(expr, msg) \
    if ((expr) == NULL) { \
        fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__, __LINE__,   \
                RESET);                                                                            \
        exit(1);\
    }

#define STR_ASSERT(string1, string2, msg)                                                          \
    if (strcmp(string1, string2) != 0)                                                             \
    {                                                                                              \
        fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__, __LINE__,   \
                RESET);                                                                            \
        exit(1);                                                                                   \
    }

#define TODO(string1)                                                                              \
    fprintf(stderr, "%sTODO: %s\n@file: %s:%d%s\n", YELLOW, string1, __FILE__, __LINE__, RESET);   \
    exit(1)

#define UNUSED(x) (void)(x)

#define RED      "\x1b[31m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define PINK     "\x1b[35m"
#define CYAN     "\x1b[36m"

//
#define RESET    "\x1b[0m"
//
#define BOLD     "\x1b[1m"
#define FAINT    "\x1b[2m"
//
#define BLACK    "\x1b[30m"
#define WHITE    "\x1b[37m"
#define DEFAULT  "\x1b[39m"
#define LGRAY    "\x1b[90m"
#define LRED     "\x1b[91m"
#define LGREEN   "\x1b[92m"
#define LYELLOW  "\x1b[93m"
#define LBLUE    "\x1b[94m"
#define LMAGENTA "\x1b[95m"
#define LCYAN    "\x1b[96m"
#define LWHITE   "\x1b[97m"
// clang-format on

} // namespace rotate

#endif // ROTATE_DEFINE
