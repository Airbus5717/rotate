#include "../include/common.hpp"

#include "../fe/token.hpp"
//* USEFUL COMMON UTILS FOR ROTATE-LANG

namespace rotate
{

char *
strndup(cstr src, const usize length)
{
    char *res = new char[length];
    ASSERT_NULL(res, "failed mem allocation");
    for (usize i = 0; i < length; ++i)
    {
        res[i] = src[i];
    }
    return res;
}

void
log_stage(cstr str)
{
    fprintf(stderr, "[%sSTAGE%s]: %s\n", LRED, RESET, str);
}

void
log_error(cstr str)
{
    fprintf(stderr, "[%sERROR%s]: %s\n", LRED, RESET, str);
}

void
exit_error(cstr str)
{
    log_error(str);
    exit(1);
}

void
log_warn(cstr str)
{
    fprintf(stderr, "[%sWARN%s] : %s\n", LYELLOW, RESET, str);
}

void // NOTE(5717): basically a print for debug builds
log_debug(cstr str)
{
#if DEBUG
    fprintf(stderr, "[%sDEBUG%s]: %s\n", LYELLOW, RESET, str);
#else
    UNUSED(str);
#endif
}

void
log_info(cstr str)
{
    fprintf(stderr, "[%sINFO%s] : %s\n", LGREEN, RESET, str);
}

// NOTE: func definition in ./frontend/include/lexer.hpp
void
log_token(FILE *output, const Token tkn, cstr str)
{
    fprintf(output, "[TOKEN]: idx: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index, tkn.length,
            tkn_type_describe(tkn.type), tkn.length, str + tkn.index);
}

uint
get_digits_from_number(const uint num)
{
    // TODO: Test this algorithm
    return (uint)floor(log10(num) + 1);
}

u8
bit_set(const u8 field, const u8 n)
{
    // NOTE(5717): n should be [1:8],
    // otherwise it is ignored
    return (field | (u8)((u8)1 << n));
}

u8
bit_clear(const u8 field, const u8 n)
{
    // NOTE(5717): n should be [1:8]
    // otherwise it is ignored
    return (field & (u8)(~((u8)1 << n)));
}

u8
bit_is_set(const u8 field, const u8 n)
{
    // NOTE(5717): returns non zero for true else 0 for false
    return ((field >> n) & 1);
}

// Custom ArrayList

} // namespace rotate
