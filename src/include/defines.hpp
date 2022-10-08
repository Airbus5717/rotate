#ifndef ROTATE_DEFINE
#define ROTATE_DEFINE

// C++ standard library
#include <string>
#include <vector>

// C standard library
#include <cassert>
#include <cctype>
#include <climits>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// header for stack allocation
#include <alloca.h>

namespace rotate
{

#define VERSION "0.0.1"
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef unsigned long long int usize;
typedef signed long long int isize;

#define ASSERT(expr, msg)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            std::fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,    \
                         __LINE__, RESET);                                                         \
            std::exit(1);                                                                          \
        }                                                                                          \
    } while (0)

#define ASSERT_CMP(expr1, expr2, msg)                                                              \
    do                                                                                             \
    {                                                                                              \
        if (expr1 != expr2)                                                                        \
        {                                                                                          \
            std::fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,    \
                         __LINE__, RESET);                                                         \
            std::exit(1);                                                                          \
        }                                                                                          \
    } while (0)

#define ASSERT_NULL(expr, msg)                                                                     \
    do                                                                                             \
    {                                                                                              \
        if ((expr) == NULL)                                                                        \
        {                                                                                          \
            std::fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,    \
                         __LINE__, RESET);                                                         \
            std::exit(1);                                                                          \
        }                                                                                          \
    } while (0)

#define STR_ASSERT(string1, string2, msg)                                                          \
    do                                                                                             \
    {                                                                                              \
        if (std::strcmp(string1, string2) != 0)                                                    \
        {                                                                                          \
            std::fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,    \
                         __LINE__, RESET);                                                         \
            std::exit(1);                                                                          \
        }                                                                                          \
    } while (0)

#define ASSERT_RET_FAIL(expr, msg)                                                                 \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            std::fprintf(stderr, "%sAssert failure: %s\n@file: %s:%d%s\n", LRED, msg, __FILE__,    \
                         __LINE__, RESET);                                                         \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
    } while (0)

#define TODO(string1)                                                                              \
    do                                                                                             \
    {                                                                                              \
        std::fprintf(stderr, "%sTODO: %s\n@file: %s:%d%s\n", YELLOW, string1, __FILE__, __LINE__,  \
                     RESET);                                                                       \
        std::exit(1);                                                                              \
    } while (0)

#define UNUSED(x) (void)(x)
#define EXIT_DONE 3
// terminal colors
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define PINK   "\x1b[35m"
#define CYAN   "\x1b[36m"
//
#define RESET "\x1b[0m"
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

} // namespace rotate

#endif // ROTATE_DEFINE
