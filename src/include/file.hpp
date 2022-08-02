#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    const char *name;
    const char *contents;
    const usize length = 0; // contents length

    file_t(const char *name, const char *contents, const usize length)
        : name(name), contents(contents), length(length)
    {
    }

    void log_head_file(FILE *output)
    {
        if (length == 0) return;
        ASSERT_NULL(contents, "contents is null");

        if (length > 200)
        {
            fprintf(output, "%.*s.....\n", 200, contents);
        }
        else
        {
            fprintf(output, "%s\n", contents);
        }
    }

    ~file_t()
    {
        delete[] contents;
    };
};

file_t *file_read(const char *name) noexcept;

} // namespace rotate

#endif // ROTATE_FILE
