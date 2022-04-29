#ifndef ROTATE_COMMON
#define ROTATE_COMMON

#include "defines.hpp"

namespace rotate
{

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

// TODO: Implement alternative to libc++ vector
template <typename T> class Vector
{
    usize index;
    usize capacity;
    T *arr;

    // TODO: growth rate optimizer
    static const usize growth_rate = 2;

  public:
    Vector()
    {
        this->index    = 0;
        this->capacity = 10;
        arr            = (T *)calloc(sizeof(T), capacity);
        check();
    }

    ~Vector()
    {
        if (capacity > 0 && arr) free((void *)arr);
        capacity = 0;
        arr      = NULL;
    }

    void check()
    {
        if (!arr || (index > capacity))
        {
            log_error("Init vector failure");
        }
    }

    usize size()
    {
        return index;
    }

    void push(T elem)
    {
        if (index + 1 == capacity)
        {
            capacity *= growth_rate;
            arr = (T *)realloc(arr, capacity);
        }
        arr[index++] = elem;
    }

    T operator[](usize i)
    {
        ASSERT(i < index, "out of bounds");
        return arr[i];
    }

    Vector<T> *operator=(Vector<T> *)
    {
        return this;
    }
};

} // namespace rotate

#endif // ROTATE COMMON
