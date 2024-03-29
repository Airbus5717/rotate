#pragma once

// C standard library
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace rotate
{

constexpr auto RTVERSION = "0.0.1";

typedef float f32;
typedef double f64;
typedef long double f128;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned long long usize;
typedef signed long long ssize;

static_assert(sizeof(usize) == 8, "invalid sizes");
static_assert(sizeof(ssize) == 8, "invalid sizes");

// use this for the project
typedef u32 uint;
typedef const char *cstr;
constexpr auto RUINT_MAX              = UINT32_MAX;
constexpr auto RUINT_MIN              = 0;
constexpr auto EXTRA_NULL_TERMINATORS = 3;

static_assert(EXTRA_NULL_TERMINATORS > 2, "keep the number above 2");
static_assert(RUINT_MIN == 0, "Min number should unsigned 0");
static_assert(RUINT_MAX == UINT32_MAX, "Max number");

#define ASSERT(expr, msg)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_CMP(expr1, expr2, msg)                                                              \
    do                                                                                             \
    {                                                                                              \
        if ((expr1) != (expr2))                                                                    \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_NULL(expr, msg)                                                                     \
    do                                                                                             \
    {                                                                                              \
        if ((expr) == nullptr)                                                                     \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define STR_ASSERT(string1, string2, msg)                                                          \
    do                                                                                             \
    {                                                                                              \
        if (strcmp(string1, string2) != 0)                                                         \
        {                                                                                          \
            fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,         \
                    __LINE__, RESET);                                                              \
            exit(1);                                                                               \
        }                                                                                          \
    } while (0)

#define ASSERT_RET_FAIL(expr, msg)                                                                 \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            log_error(msg);                                                                        \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
    } while (0)

#define TODO(str)                                                                                  \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, "%sTODO: %s\n@file: %s:%d%s\n", YELLOW, str, __FILE__, __LINE__, RESET);   \
        exit(1);                                                                                   \
    } while (0)

#define UNREACHABLE()                                                                              \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, "%sREACHED UNREACHABLE STAT\n@file: %s:%d%s\n", LRED, __FILE__, __LINE__,  \
                RESET);                                                                            \
        exit(1);                                                                                   \
    } while (0)

#define UNUSED(x) ((void)sizeof(x))

constexpr auto SUCCESS = 0; // EXIT_SUCCESS
constexpr auto FAILURE = 1; // EXIT_FAILURE
constexpr auto DONE    = 2; // EXIT_DONE
static_assert(SUCCESS == 0, "Exit Success code is wrong");
static_assert(FAILURE == 1, "Exit Failure code is wrong");
static_assert(DONE == 2, "Exit Done code is wrong");

#ifdef _WIN32
#define NEWLINE "\n\r"
#else
#define NEWLINE "\n"
#endif

#define expect(expr, doWhenExpected, doElse)                                                       \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            doElse;                                                                                \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
        else { doWhenExpected; }                                                                   \
    } while (0)

#define expect_terminator(doWhenExpected, doElse)                                                  \
    do                                                                                             \
    {                                                                                              \
        if (current().type != TknType::Terminator)                                                 \
        {                                                                                          \
            doElse;                                                                                \
            idx--;                                                                                 \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
        else { doWhenExpected; }                                                                   \
    } while (0)

// terminal colors
constexpr auto RESET    = "\x1b[0m";
constexpr auto BOLD     = "\x1b[1m";
constexpr auto FAINT    = "\x1b[2m";
constexpr auto GREEN    = "\x1b[32m";
constexpr auto YELLOW   = "\x1b[33m";
constexpr auto BLUE     = "\x1b[34m";
constexpr auto PINK     = "\x1b[35m";
constexpr auto CYAN     = "\x1b[36m";
constexpr auto BLACK    = "\x1b[30m";
constexpr auto WHITE    = "\x1b[37m";
constexpr auto DEFAULT  = "\x1b[39m";
constexpr auto LGRAY    = "\x1b[90m";
constexpr auto LRED     = "\x1b[91m";
constexpr auto LGREEN   = "\x1b[92m";
constexpr auto LYELLOW  = "\x1b[93m";
constexpr auto LBLUE    = "\x1b[94m";
constexpr auto LMAGENTA = "\x1b[95m";
constexpr auto LCYAN    = "\x1b[96m";
constexpr auto LWHITE   = "\x1b[97m";

} // namespace rotate
