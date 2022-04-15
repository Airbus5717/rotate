#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    string name;
    string contents;

    file_t(const char *name, const char *contents) : name(name), contents(contents)
    {
    }

    ~file_t() = default;
};

extern "C" file_t *file_read(const char *name) noexcept;

} // namespace rotate

#endif // ROTATE_FILE
