#include "include/common.hpp"

//* USEFUL COMMON UTILS FOR ROTATE-LANG

namespace rotate
{

bool is_space_rotate(char i)
{
    return i == ' ' || i == '\n';
}

void log_error(const char *str)
{
    std::cout << "ERROR: " << str << std::endl;
}

} // namespace rotate
