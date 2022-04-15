#include "include/file.hpp"

namespace rotate
{
// This function is written in C-style (C++ way is confusing and slower)
extern "C" file_t *file_read(const char *name) noexcept
{
    // open file
    FILE *file = fopen(name, "rb");
    if (!file)
    {
        // perror("file open error");
        // display error message if file does not exist
        fprintf(stderr, "%s%s%s: %serror:%s %s%s\n", GREEN, BOLD, name, RED, LCYAN, "No such file",
                RESET);
        return NULL;
    }

    // Calculate the file length
    fseek(file, 0, SEEK_END);
    if (!ftell(file))
    {
        fprintf(stderr, "%s%s%s: %serror:%s %s%s\n", GREEN, BOLD, name, RED, LCYAN, "File empty",
                RESET);
        // printf("\x1b[31merror: file empty \x1b[0m\n");
        fclose(file);
        return NULL;
    }
    const usize length = (usize)ftell(file);

    if (length > UINT_MAX)
    {
        fprintf(stderr, "%s%s%s: %serror:%s %s%s\n", GREEN, BOLD, name, RED, LCYAN,
                "File too large", RESET);
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
        fprintf(stderr, "%s%s%s: %serror:%s %s%s\n", GREEN, BOLD, name, RED, LCYAN,
                "file reading error", RESET);
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
        fprintf(stderr, "%s%s%s: %serror:%s %s%s\n", GREEN, BOLD, name, RED, LCYAN,
                "only ascii text files are supported for compilation", RESET);
        fclose(file);
        free(buffer);
        return NULL;
    }

    // Close the file
    fclose(file);
    file_t *file_ = new file_t(name, buffer);
    return file_;
}

} // namespace rotate