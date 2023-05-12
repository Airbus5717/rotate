#pragma once

#include "common.hpp"

namespace rotate
{

enum class valid : u8
{
    success,
    failure,
};

struct file_t
{
    const char *name;
    const char *contents;
    const uint length = 0; // contents length
    valid valid_code;

    file_t(const char *name, const char *contents, const uint length, valid valid_code)
        : name(name), contents(contents), length(length), valid_code(valid_code)
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

file_t file_read(const char *name) noexcept;

} // namespace rotate
