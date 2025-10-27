#pragma once

#include <functional>
#include <optional>
#include <vector>

#include "../board_image.h"
#include "../types.h"

struct SolutionInfo
{
    struct MineProb
    {
        Point point;
        double prob;
    };

    std::vector<Point> mines;
    std::vector<Point> clears;
    uint32_t numValidSolutions;
    std::vector<MineProb> mineProbs;
    double outsideMineProb;

    void initMineProbs(const std::vector<Point>& points)
    {
        mineProbs.reserve(points.size());
        for (Point pt : points)
            mineProbs.push_back({pt, 0});
    }

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

using Solver = std::function<std::optional<SolutionInfo>(const BoardImage& image)>;
