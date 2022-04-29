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
    T *arr;

    // TODO: growth rate optimizer
    static const usize growth_rate = 2;

  public:
    Vector()
    {
        this->index    = 0;
        this->capacity = 10;

        arr = (T *)calloc(sizeof(T), capacity);
        ASSERT_NULL(arr, "Vector init failure");
    }

    ~Vector()
    {
        if (capacity > 0 && arr) free((void *)arr);
        capacity = 0;
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

    u8 push_back(T elem)
    {
        ASSERT(capacity < UINT32_MAX, "too large vector");
        if (index + 1 == capacity)
        {
            T *tmp = (T *)realloc(arr, growth_rate * capacity);
            if (tmp == NULL) return EXIT_FAILURE;
            capacity *= growth_rate;
            arr = tmp;
        }

        arr[index++] = elem;
        return EXIT_SUCCESS;
    }

    const T &operator[](usize i)
    {
        ASSERT(i < index, "out of bounds");
        return arr[i];
    }

    const T &at(usize i)
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
