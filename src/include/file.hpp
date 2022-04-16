#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    const char *name;
    const usize length;
    const char *contents;

    file_t(const char *name, const usize length, const char *contents)
        : name(name), length(length), contents(contents)
    {
    }

    ~file_t()
    {
        free((void *)contents);
    };
};

file_t *file_read(const char *name) noexcept;

} // namespace rotate

#endif // ROTATE_FILE
