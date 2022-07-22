#include "include/file.hpp"

namespace rotate
{

// This function is written in C-style (C++ way is confusing and slower)
file_t *file_read(const char *name) noexcept
{
    u16 len              = strlen(name);
    const char *file_ext = &(name)[len - 3];
    if (strcmp(file_ext, ".vr") != 0)
    {
        fprintf(stderr, "%s%serror:%s file name: `%s` must end with `.vr`%s\n", BOLD, LRED, WHITE,
                name, RESET);
        return NULL;
    }

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

    if (length + 3 > UINT32_MAX)
    {
        log_error("File too large");
        fclose(file);
        return NULL;
    }
    // rewind the fseek to the beginning of the file
    rewind(file);

    // Read the file into a buffer
    char *buffer = new char[length + 3];
    if (!buffer) exit_error("Memory allocation failure");

    // get file contents
    if (fread(buffer, sizeof(char), length, file) != length)
    {
        log_error("Read file error");
        fclose(file);
        delete[] buffer;
        return NULL;
    }

    // add NULL charactor (3 for extra safety)
    for (u8 i = 0; i < 3; i++)
        buffer[length + i] = '\0';

    // simple validator (check first char if it is a visible ascii or is_space(without tabs))
    if ((buffer[0] < ' ' || buffer[0] > '~') && !is_space_rotate(buffer[0]))
    {
        log_error("Only ascii text files are supported for compilation");
        fclose(file);
        delete[] buffer;
        return NULL;
    }

    // Close the file
    fclose(file);
    return new file_t(name, buffer, length);
}

} // namespace rotate
