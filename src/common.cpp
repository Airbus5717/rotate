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
    fprintf(stderr, "[%sTOKEN%s]: type: %s, value: `%s`\n", LYELLOW, RESET,
            tkn_type_describe(tkn.type), get_keyword_or_type(str, tkn));
}

} // namespace rotate
