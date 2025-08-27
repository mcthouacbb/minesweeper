#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>

struct Point
{
    uint32_t x;
    uint32_t y;

    constexpr bool operator==(const Point& other) const noexcept = default;
    constexpr bool operator!=(const Point& other) const noexcept = default;
};

struct PointHash
{
    size_t operator()(const Point& point) const noexcept
    {
        return std::hash<uint64_t>{}(
            static_cast<uint64_t>(point.x) | (static_cast<uint64_t>(point.y) << 32));
    }
};

inline std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

struct BoardData
{
    uint32_t width;
    uint32_t height;
    uint32_t numMines;
};
