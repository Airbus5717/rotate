#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    const char *name;
    const char *contents;
    const usize length; // contents length

    file_t(const char *name, const char *contents, const usize length)
        : name(name), contents(contents), length(length)
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
