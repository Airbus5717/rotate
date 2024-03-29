#include "include/file.hpp"
#include "include/defines.hpp"

namespace rotate
{

/// NOTE:
/// the whole file will be read at once
/// to avoid potential problems with the
/// filesystem during reading as developers
/// may modify the files during reading
file_t
file_read(cstr name) noexcept
{
    usize len = strlen(name);

    cstr file_ext = &(name)[len - 3];
    if (strcmp(file_ext, ".vr") != 0)
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

    // NOTE: use max unsigned int possible to avoid overflow
    const usize length = (usize)ftell(file);

    // NOTE: hardcoded 3 null-terminators
    if (length > (UINT_MAX - 3))
    {
        log_error("File is too large");
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

    // add NULL charactor (EXTRA_NULL_TERMINATORS for extra safety)
    for (u8 i = 0; i < EXTRA_NULL_TERMINATORS; i++)
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

    // NOTE:
    return file_t(name, buffer, (uint)length, valid::success);
}

} // namespace rotate
