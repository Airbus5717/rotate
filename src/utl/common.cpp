#include "../include/common.hpp"

#include "../fe/token.hpp"
//* USEFUL COMMON UTILS FOR ROTATE-LANG

namespace rotate
{

void
log_stage(const char *str)
{
    fprintf(rstderr, "[%sSTAGE%s]: %s\n", LRED, RESET, str);
}

void
log_error(const char *str)
{
    fprintf(rstderr, "[%sERROR%s]: %s\n", LRED, RESET, str);
}

void
exit_error(const char *str)
{
    log_error(str);
    exit(1);
}

void
log_warn(const char *str)
{
    fprintf(rstderr, "[%sWARN%s] : %s\n", LYELLOW, RESET, str);
}

void // NOTE(5717): basically a print for debug builds
log_debug(const char *str)
{
#if DEBUG
    fprintf(stderr, "[%sDEBUG%s]: %s\n", LYELLOW, RESET, str);
#else
    UNUSED(str);
#endif
}

void
log_info(const char *str)
{
    fprintf(rstderr, "[%sINFO%s] : %s\n", LGREEN, RESET, str);
}

// NOTE: func definition in ./frontend/include/lexer.hpp
void
log_token(FILE *output, const Token tkn, const char *str)
{
    fprintf(output, "[TOKEN]: idx: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index, tkn.length,
            tkn_type_describe(tkn.type), tkn.length, str + tkn.index);
}

UINT
get_digits_from_number(const UINT num)
{
    // TODO: check
    return (UINT)std::floor(std::log10(num) + 1);
}

u8
bit_set(const u8 field, const u8 n)
{
    // NOTE(5717): n should be [1:8],
    // otherwise it is ignored
    return (field | ((u8)1 << n));
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
} // namespace rotate
