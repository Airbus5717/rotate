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
    usize index    = 0;
    usize capacity = 0;
    T *arr         = NULL;

    // TODO: growth rate optimizer
    static const usize growth_rate = 2;

  public:
    Vector()
    {
        TODO("Fix");
        this->index    = 0;
        this->capacity = 10;
        arr            = (T *)calloc(sizeof(T), capacity);
        if (!arr) exit_error("Vector is null");
    }

    ~Vector()
    {
        if (capacity > 0 && arr) free((void *)arr);
        capacity = 0;
        arr      = NULL;
    }

    // void check()
    // {
    //     if (!arr) log_error("Vector is null");
    //     if (index > capacity) log_error("out of bounds");
    // }

    usize size()
    {
        return index;
    }

    u8 push(T elem)
    {
        if (index + 2 == ULONG_LONG_MAX) return EXIT_FAILURE;
        if (index + 1 == capacity)
        {
            capacity *= growth_rate;
            arr = (T *)realloc(arr, capacity);
            if (!arr) return EXIT_FAILURE;
        }
        arr[index++] = elem;
        return EXIT_SUCCESS;
    }

    T operator[](usize i)
    {
        ASSERT(i < index, "out of bounds");
        return arr[i];
    }

    T at(usize i)
    {
        ASSERT(i < index, "out of bounds");
        return arr[i];
    }

    Vector<T> *operator=(Vector<T> *)
    {
        return this;
    }

    T *getElements()
    {
        return arr;
    }
};

} // namespace rotate

#endif // ROTATE COMMON
