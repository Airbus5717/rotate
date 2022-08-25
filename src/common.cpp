#include "include/common.hpp"

#include "frontend/include/token.hpp"

//* USEFUL COMMON UTILS FOR ROTATE-LANG

namespace rotate
{

bool is_space_rotate(const char i)
{
    return i == ' ' || i == '\n';
}

void log_error(const char *str)
{
    fprintf(stderr, "[%sERROR%s] : %s\n", LRED, RESET, str);
}

void exit_error(const char *str)
{
    log_error(str);
    exit(1);
}

void log_warn(const char *str)
{
    fprintf(stderr, "[%sWARN%s] : %s\n", LYELLOW, RESET, str);
}

void log_debug(const char *str)
{
#if DEBUG
    fprintf(stderr, "[%sDEBUG%s] : %s\n", LYELLOW, RESET, str);
#else
    UNUSED(str);
#endif
}

void log_info(const char *str)
{
    fprintf(stderr, "[%sINFO%s] : %s\n", LGREEN, RESET, str);
}

// func definition in ./frontend/include/lexer.hpp
void log_token(FILE *output, const rotate::Token tkn, const char *str)
{
    fprintf(output, "[TOKEN]: idx: %u, len: %u, type: %s, val: `%.*s`\n", tkn.index, tkn.length,
            tkn_type_describe(tkn.type), tkn.length, str + tkn.index);
}

u32 get_digits_from_number(u32 num)
{
    return (num < 10)           ? 1
           : (num < 100)        ? 2
           : (num < 1000)       ? 3
           : (num < 10000)      ? 4
           : (num < 100000)     ? 5
           : (num < 1000000)    ? 6
           : (num < 10000000)   ? 7
           : (num < 100000000)  ? 8
           : (num < 1000000000) ? 9
                                : 10;
}

bool is_token_type_length_variable(token_type type)
{
    switch (type)
    {
        case token_type::Integer:
        case token_type::HexInteger:
        case token_type::BinaryInteger:
        case token_type::Float:
        case token_type::BuiltinFunc:
        case token_type::Identifier:
        case token_type::String:
        case token_type::Char:
            return true;
        default:
            return false;
    }
}

} // namespace rotate
