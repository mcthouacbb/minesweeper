#pragma once

#include <bit>
#include <cstdint>

inline int poplsb(uint64_t& c)
{
    int bit = std::countr_zero(c);
    c &= c - 1;
    return bit;
}
