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
    }

    ~Array() { free(static_cast<void *>(m_data)); }

    void append(T element)
    {
        if (m_count == m_capacity)
        {
            m_capacity <<= 1;
            T *new_elements = static_cast<T *>(malloc(sizeof(T) * m_capacity));
            ASSERT_NULL(new_elements, "Array resize");
            for (usize i = 0; i < m_count; i++)
                new_elements[i] = m_data[i];
            free(static_cast<void *>(m_data));
            m_data = new_elements;
        }
        m_data[m_count++] = element;
    }

    T at(usize index) { return m_data[index]; }

    usize count() { return m_count; }
};

} // namespace rotate
