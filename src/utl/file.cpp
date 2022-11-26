#include "../include/file.hpp"

namespace rotate
{

file_t
file_read(const char *name) noexcept
{

    usize len = strlen(name);

    const char *file_ext = &(name)[len - 3];
    if (strcasecmp(file_ext, ".vr") != 0)
    {
        fprintf(stderr, "%s%serror:%s file name: `%s` must end with `.vr`%s\n", BOLD, LRED, WHITE,
                name, RESET);

        return file_t(nullptr, nullptr, 0, valid::failure);
    }

    // open file
    FILE *file = fopen(name, "rb");
    if (!file)
    {
        // display error message if file does not exist
        log_error("File does not exist");
        return file_t(nullptr, nullptr, 0, valid::failure);
    }

    // Calculate the file length
    fseek(file, 0, SEEK_END);
    if (!ftell(file))
    {
        log_error("File is empty");
        fclose(file);
        return file_t(nullptr, nullptr, 0, valid::failure);
    }
    const usize length = (usize)ftell(file);

    if (length > (Uint_MAX - 3))
    {
        log_error("File too large");
        fclose(file);
        return file_t(nullptr, nullptr, 0, valid::failure);
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
        return file_t(nullptr, nullptr, 0, valid::failure);
    }

    // add NULL charactor (3 for extra safety)
    for (u8 i = 0; i < 3; i++)
        buffer[length + i] = '\0';

    // simple validator (check first char if it is a visible ascii or is_space(without tabs))
    if ((buffer[0] < ' ' || buffer[0] > '~') && isspace(buffer[0]))
    {
        log_error("Only ascii text files are supported for compilation");
        fclose(file);
        delete[] buffer;
        return file_t(nullptr, nullptr, 0, valid::failure);
    }

    // Close the file
    fclose(file);
    return file_t(name, buffer, (Uint)length, valid::success);
}

} // namespace rotate
