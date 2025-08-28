#pragma once

#include <vector>

#include "../types.h"

struct Solution
{
    std::vector<Point> mines;
    std::vector<Point> clears;

    bool isMine(Point p) const
    {
        for (Point mine : mines)
        {
            if (mine == p)
                return true;
        }
        return false;
    }

    bool isClear(Point p) const
    {
        for (Point clear : clears)
        {
            if (clear == p)
                return true;
        }
        return false;
    }
};
