#ifndef ROTATE_COMMON
#define ROTATE_COMMON

#include "defines.hpp"

namespace rotate
{

template <typename T> class ArrayList
{
    T *arr;

  public:
    ArrayList()
    {
        TODO("ArrayList impl");
        arr = (T *)calloc(sizeof(T), 10);
    }

    ~ArrayList()
    {
        free((void *)arr);
    }
};
/*
 *  Utilites
 */
bool is_space_rotate(char i);

//
void log_error(const char *str);
void exit_error(const char *str);
void log_debug(const char *str);
void log_info(const char *str);

} // namespace rotate

#endif // ROTATE COMMON
