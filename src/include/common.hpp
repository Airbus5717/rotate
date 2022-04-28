#ifndef ROTATE_COMMON
#define ROTATE_COMMON

#include "defines.hpp"

namespace rotate
{

template <typename T> class Vector
{
    T *arr;

  public:
    Vector()
    {
        TODO("Vector impl");
        arr = (T *)calloc(sizeof(T), 10);
    }

    ~Vector()
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

//
u32 get_digits_from_number(u32 v);

} // namespace rotate

#endif // ROTATE COMMON
