#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    string name;
    usize length;
    const char *contents;

    file_t(const char *name, usize length, const char *contents)
        : name(name), length(length), contents(contents)
    {
    }

    ~file_t()
    {
        length = 0;
        free((void *)contents);
    };
};

extern "C" file_t *file_read(const char *name) noexcept;

} // namespace rotate

#endif // ROTATE_FILE
