#pragma once

#include "../board_image.h"
#include "../util/bitset.h"

#include <bit>
#include <iostream>
#include <unordered_map>

namespace solvers::brute_force
{

struct Constraint
{
    uint64_t mask;
    uint32_t sum;

    void addIndex(uint32_t idx)
    {
        mask |= 1ull << idx;
    }
};

void solve(const BoardImage& image)
{
    std::unordered_map<Point, uint32_t, PointHash> unclearedIndices;
    std::vector<Point> uncleared;
    std::vector<Constraint> constraints;

    for (const auto& cell : image.numberedCells())
    {
        Constraint constraint = {};
        constraint.sum = cell.adjacentMines;
        for (auto neighbor : cell.unclearedNeighbors)
        {
            uint32_t idx;
            if (unclearedIndices.count(neighbor) == 0)
            {
                idx = uncleared.size();
                uncleared.push_back(neighbor);
                unclearedIndices.insert({neighbor, idx});
            }
            else
            {
                idx = unclearedIndices.at(neighbor);
            }
            constraint.addIndex(idx);
        }
        constraints.push_back(constraint);
    }

    if (uncleared.size() >= 40)
    {
        std::cerr << uncleared.size() << " is Too many uncleared mines for brute force solver"
                  << std::endl;
        return;
    }

    uint64_t alwaysMines = (1ull << uncleared.size()) - 1;
    uint64_t alwaysClear = (1ull << uncleared.size()) - 1;
    for (uint64_t mines = 0; mines < 1ull << uncleared.size(); mines++)
    {
        bool valid = true;
        for (const auto& constraint : constraints)
        {
            if (std::popcount(constraint.mask & mines) != constraint.sum)
            {
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;

        alwaysMines &= mines;
        alwaysClear &= ~mines;
    }

    if (!alwaysMines)
    {
        std::cout << "No squares are always mines" << std::endl;
    }
    else
    {
        while (alwaysMines)
        {
            int mine = poplsb(alwaysMines);
            std::cout << "Square " << uncleared[mine] << " is always a mine" << std::endl;
        }
    }

    if (!alwaysClear)
    {
        std::cout << "No squares are always clear" << std::endl;
    }
    else
    {
        while (alwaysClear)
        {
            int clear = poplsb(alwaysClear);
            std::cout << "Square " << uncleared[clear] << " is always clear" << std::endl;
        }
    }
}

}
