#ifndef ROTATE_FILE
#define ROTATE_FILE

#include "common.hpp"

namespace rotate
{

struct file_t
{
    string name;
    std::string_view contents;

    file_t(const char *name)
    {
        string tmp{name};
        this->name = tmp;
    }

    ~file_t()
    {
        name.clear();
        free((void *)contents.data());
    }
};

file_t *file_read(const char *name);

} // namespace rotate

#endif // ROTATE_FILE
