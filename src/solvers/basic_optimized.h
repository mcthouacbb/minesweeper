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
    constexpr uint32_t MAX_UNCLEARED = 64;

    std::unordered_map<Point, bool, PointHash> earlySolves;

    // solve trivially recognizable cases
    // guaranteed mine
    // ..O
    // .1.
    // ...

    // X = mine
    // guaranteed clear
    // ..X
    // .1O
    // ..O
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

    // bookkeeping information
    // each unknown cell is assigned an index
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

    // loop through all possible mine configurations
    // bits of the number represent whether a cell is a mine or clear
    for (uint64_t mines = 0; mines < 1ull << uncleared.size();)
    {
        int jumpBit = 0;
        bool valid = true;
        // skip invalid configurations
        // If a constraint is unmatched, we don't need to check any of the
        // configurations that don't change the lowest index bit in that constraint
        // so we can skip all configurations that don't change the lowest bit in the constraint
        for (const auto& constraint : constraints)
        {
            // keep track of the constraint with the highest lowest index bit
            if (std::popcount(constraint.mask & mines) != constraint.sum)
            {
                valid = false;
                jumpBit = std::max(jumpBit, std::countr_zero(constraint.mask));
            }
        }
        if (!valid)
        {
            // skip configurations that can't possibly satisfy the constraints
            mines &= ~((1ull << jumpBit) - 1);
            mines += 1ull << jumpBit;
            continue;
        }

        solution.numValidSolutions++;
        // keep track of which cells were always mines/always clear
        alwaysMines &= mines;
        alwaysClear &= ~mines;

        mines++;
    }

    // convert back from indices to squares
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
