#include "include/file.hpp"

namespace rotate
{
// This function is written in C-style (C++ way is confusing and slower)
file_t *file_read(const char *name) noexcept
{
    // open file
    FILE *file = fopen(name, "rb");
    if (!file)
    {
        // display error message if file does not exist
        log_error("File does not exist");
        return NULL;
    }

    // Calculate the file length
    fseek(file, 0, SEEK_END);
    if (!ftell(file))
    {
        log_error("File is empty");
        fclose(file);
        return NULL;
    }
    const usize length = (usize)ftell(file);

    if (length > UINT_MAX)
    {
        log_error("File too large");
        fclose(file);
        return NULL;
    }
    // rewind the fseek to the beginning of the file
    rewind(file);

    // Read the file into a buffer
    char *buffer = (char *)malloc(length + 3);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    // get file contents
    if (fread(buffer, sizeof(char), length, file) != length)
    {
        log_error("read file error");
        fclose(file);
        free(buffer);
        return NULL;
    }

    // add NULL charactor (3 for extra safety)
    buffer[length]     = '\0';
    buffer[length + 1] = '\0';
    buffer[length + 2] = '\0';

    // simple validator (check first char if it is a visible ascii or is_space(without tabs))
    if ((buffer[0] < ' ' || buffer[0] > '~') && !is_space_rotate(buffer[0]))
    {
        log_error("only ascii text files are supported for compilation");
        fclose(file);
        free(buffer);
        return NULL;
    }

    // Close the file
    fclose(file);
    file_t *file_ = new file_t(name, length, buffer);
    return file_;
}

} // namespace rotate
