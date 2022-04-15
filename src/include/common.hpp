#ifndef ROTATE_COMMON
#define ROTATE_COMMON

// TODO: replace C++ stdlib
#include <vector>

//
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

typedef unsigned long long usize;
typedef signed   long long isize;
// clang-format on

#define ASSERT(expr1, expr2, msg)                                                                  \
    if (expr1 != expr2)                                                                            \
    {                                                                                              \
        fprintf(stderr, "%sAssert failure: %s\n@line: %d\n@file: %s%s\n", LRED, msg, __LINE__,     \
                __FILE__, RESET);                                                                  \
        exit(1);                                                                                   \
    }

#define STR_ASSERT(string1, string2, msg)                                                          \
    if (strcmp(string1, string2) != 0)                                                             \
    {                                                                                              \
        fprintf(stderr, "%sAssert failure: %s\n@line: %d\n@file: %s%s\n", LRED, msg, __LINE__,     \
                __FILE__, RESET);                                                                  \
        exit(1);                                                                                   \
    }

#define TODO(string1)                                                                              \
    fprintf(stderr, "%sTODO: %s\n@line: %d\n@file: %s%s\n", YELLOW, string1, __LINE__, __FILE__,   \
            RESET);                                                                                \
    exit(1)

#define UNUSED(x) (void)(x)

#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define PINK   "\x1b[35m"
#define CYAN   "\x1b[36m"
#define RESET  "\x1b[0m"
//
#define BOLD  "\x1b[1m"
#define FAINT "\x1b[2m"
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

/*
 *  Utilites
 */
bool is_space_rotate(char i);
void log_error(const char *str);

} // namespace rotate

#endif // ROTATE COMMON
