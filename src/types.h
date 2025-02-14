#pragma once

#include <cstdint>

struct Point
{
    uint32_t x;
    uint32_t y;

    constexpr bool operator==(const Point& other) const noexcept = default;
    constexpr bool operator!=(const Point& other) const noexcept = default;
};

struct BoardData
{
    uint32_t width;
    uint32_t height;
    uint32_t numMines;
};
