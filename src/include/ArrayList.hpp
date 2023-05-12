#pragma once

#include "defines.hpp"

namespace rotate
{

template <typename T>
class ArrayList
{
    T *m_data        = nullptr;
    usize m_count    = 0;
    usize m_capacity = 0;

    public:
    ArrayList(usize init_count = 10)
    {
        m_data     = new T[init_count]();
        m_count    = 0;
        m_capacity = init_count;
    }

    ~ArrayList() { delete[] m_data; }

    u8 append(T element)
    {
        if (m_count >= m_capacity) resize(m_capacity << 1);

        m_data[m_count++] = element;
        return SUCCESS;
    }

    usize count() const { return m_count; }

    usize capacity() const { return m_capacity; }

    // if new_size is smaller, then it is ignored
    u8 resize(usize new_size)
    {
        if (new_size < m_capacity) return SUCCESS;
        usize new_cap = new_size;
        T *new_blk    = new T[new_cap];
        T *old_blk    = m_data;

        ASSERT_NULL(new_blk, "resize allocation failed");

        for (usize i = 0; i < m_count; ++i)
            new_blk[i] = old_blk[i];

        m_capacity = new_cap;
        delete[] m_data;
        m_data = new_blk;
        return new_blk ? SUCCESS : FAILURE;
    }

    // // NOTE: const & bounds checked
    // const T &at(usize i) const
    // {
    //     ASSERT(i < m_count, "Bounds checker");
    //     return m_data[i];
    // }

    // // const & Without bounds checking
    // const T &operator[](usize i) const
    // {
    //     return m_data[i];
    // }

    // NOTE: bounds checked
    T &at(usize i) const
    {
        ASSERT(i < m_count, "Bounds checker");
        return m_data[i];
    }

    // Without bounds checking
    T &operator[](usize i) const { return m_data[i]; }

    private:
    const usize element_size = sizeof(T);
};

} // namespace rotate
