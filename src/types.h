#pragma once

#include <cstdint>

struct Point
{
    uint32_t x;
    uint32_t y;

    constexpr bool operator==(const Point& other) const noexcept = default;
    constexpr bool operator!=(const Point& other) const noexcept = default;
};

struct PointHash
{
    size_t operator()(const Point& pt) const
    {
        return std::hash<uint64_t>{}(static_cast<uint64_t>(pt.x) | (static_cast<uint64_t>(pt.y) << 32));
    }
};

struct BoardData
{
    uint32_t width;
    uint32_t height;
    uint32_t numMines;
};
