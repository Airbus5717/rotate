#include "include/common.hpp"

#include "frontend/include/token.hpp"

//* USEFUL COMMON UTILS FOR ROTATE-LANG

namespace rotate
{

bool is_space_rotate(char i)
{
    return i == ' ' || i == '\n';
}

void log_error(const char *str)
{
    fprintf(stderr, "[%sERROR%s]: %s\n", LRED, RESET, str);
}

void exit_error(const char *str)
{
    log_error(str);
    exit(1);
}

void log_debug(const char *str)
{
    fprintf(stderr, "[%sDEBUG%s]: %s\n", LYELLOW, RESET, str);
}

void log_info(const char *str)
{
    fprintf(stderr, "[%sINFO%s] : %s\n", LGREEN, RESET, str);
}

void log_token(const char *str, const rotate::token &tkn)
{
    auto string = get_keyword_or_type(str, tkn);
    fprintf(stderr, "[%sTOKEN%s]:index: %zu, length: %zu, type: %s, value: `%s`\n", LYELLOW, RESET,
            tkn.index, tkn.length, tkn_type_describe(tkn.type), string);
    if (is_allocated(tkn.type)) free((void *)string);
}

bool is_allocated(token_type type)
{
    switch (type)
    {
        case TknTypeInteger:
        case TknTypeHexInteger:
        case TknTypeBinaryInteger:
        case TknTypeFloat:
        case TknTypeBuiltinFunc:
        case TknTypeIdentifier:
        case TknTypeString:
        case TknTypeChar:
            return true;
        default:
            return false;
    }
}

} // namespace rotate
