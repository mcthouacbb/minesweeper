#pragma once

#include "../board_image.h"
#include "../util/bitset.h"
#include "brute_force.h"
#include "solution_info.h"

#include <bit>
#include <unordered_map>

namespace solvers::basic_optimized
{

inline std::optional<SolutionInfo> solve(const BoardImage& image)
{
    constexpr uint32_t MAX_UNCLEARED = 35;

    std::unordered_map<Point, bool, PointHash> earlySolves;

    while (true)
    {
        bool foundSolve = false;
        for (const auto& cell : image.numberedCells())
        {
            uint32_t knownMines = 0;
            uint32_t knownClears = 0;
            for (Point neighbor : cell.unclearedNeighbors)
            {
                if (earlySolves.count(neighbor) > 0)
                {
                    if (earlySolves.at(neighbor))
                        knownMines++;
                    else
                        knownClears++;
                }
            }

            // all cells that are not already known are mines
            if (cell.adjacentMines == cell.unclearedNeighbors.size() - knownClears)
            {
                for (Point neighbor : cell.unclearedNeighbors)
                {
                    if (earlySolves.count(neighbor) == 0)
                    {
                        foundSolve = true;
                        earlySolves.insert({neighbor, true});
                    }
                }
            }
            // all cells that are not already known are clear
            else if (cell.adjacentMines == knownMines)
            {
                for (Point neighbor : cell.unclearedNeighbors)
                {
                    if (earlySolves.count(neighbor) == 0)
                    {
                        foundSolve = true;
                        earlySolves.insert({neighbor, false});
                    }
                }
            }
        }

        if (!foundSolve)
            break;
    }

    std::unordered_map<Point, uint32_t, PointHash> unclearedIndices;
    std::vector<Point> uncleared;
    std::vector<brute_force::Constraint> constraints;

    for (const auto& cell : image.numberedCells())
    {
        brute_force::Constraint constraint = {};
        uint32_t knownMines = 0;
        for (auto neighbor : cell.unclearedNeighbors)
        {
            if (earlySolves.count(neighbor) > 0)
            {
                if (earlySolves.at(neighbor))
                    knownMines++;
                continue;
            }
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
        constraint.sum = cell.adjacentMines - knownMines;
        constraints.push_back(constraint);
    }

    if (uncleared.size() >= MAX_UNCLEARED)
        return {};

    uint64_t alwaysMines = (1ull << uncleared.size()) - 1;
    uint64_t alwaysClear = (1ull << uncleared.size()) - 1;
    SolutionInfo solution = {};

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

        solution.numValidSolutions++;
        alwaysMines &= mines;
        alwaysClear &= ~mines;
    }

    while (alwaysMines)
    {
        int mine = poplsb(alwaysMines);
        solution.mines.push_back(uncleared[mine]);
    }

    while (alwaysClear)
    {
        int clear = poplsb(alwaysClear);
        solution.clears.push_back(uncleared[clear]);
    }

    for (auto [location, isMine] : earlySolves)
    {
        if (isMine)
            solution.mines.push_back(location);
        else
            solution.clears.push_back(location);
    }

    return {solution};
}

}
