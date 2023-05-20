#pragma once

#include "defines.hpp"

namespace rotate
{

template <typename T>
class Array
{
    private:
    T *m_data        = nullptr;
    usize m_count    = 0;
    usize m_capacity = 0;

    public:
    Array(usize init_size = 10)
    {
        m_count    = 0;
        m_capacity = init_size;
        m_data     = static_cast<T *>(malloc(sizeof(T) * init_size));
        ASSERT_NULL(m_data, "Array initialization failure");
    }

    ~Array() { free(static_cast<void *>(m_data)); }

    void append(T element)
    {
        if (m_count == m_capacity)
        {
            m_capacity <<= 1;
            m_data = static_cast<T *>(realloc(m_data, m_capacity * sizeof(T)));
            ASSERT_NULL(m_data, "Array resize");
        }
        m_data[m_count++] = element;
    }

    T at(usize index) { return m_data[index]; }
    T operator[](usize index) { return m_data[index]; }
    usize count() { return m_count; }
};

} // namespace rotate
