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
    uint32_t earlySolvedMines = 0;

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
                        earlySolvedMines++;
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
    solution.initMineProbs(uncleared);

    // loop through all possible mine configurations
    // bits of the number represent whether a cell is a mine or clear
    double totalWeight = 0.0;
    const uint32_t outsideMineCells = image.width() * image.height() - uncleared.size()
        - earlySolves.size() - image.zeroCells().size() - image.numberedCells().size();
    const uint32_t availableMines = image.numMines() - earlySolvedMines;
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

        // Adjust weight for the current mine configuration based on
        // the number of valid ways other mines can be configured
        // weight is proportional to N choose M, where N is
        // the number of other squares that mines can be in
        // and M is the number of mines that can be in those squares
        double currWeight = 1.0;
        for (uint32_t i = 0; i < std::popcount(mines); i++)
        {
            if (outsideMineCells + 1 + i - availableMines <= 0)
                continue;
            currWeight *= static_cast<double>(availableMines - i);
            currWeight /= static_cast<double>(outsideMineCells + 1 + i - availableMines);
        }

        totalWeight += currWeight;

        double currOutsideMineProb = static_cast<double>(availableMines - std::popcount(mines))
            / static_cast<double>(outsideMineCells);
        solution.outsideMineProb += currWeight * currOutsideMineProb;

        uint32_t minesCopy = mines;
        while (minesCopy > 0)
        {
            uint32_t mineIdx = std::countr_zero(minesCopy);
            solution.mineProbs[mineIdx].prob += currWeight;

            minesCopy &= minesCopy - 1;
        }

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

    const auto remove = [&](const SolutionInfo::MineProb& mineProb)
    {
        return solution.isClear(mineProb.point) || solution.isMine(mineProb.point);
    };
    solution.mineProbs.erase(
        std::remove_if(solution.mineProbs.begin(), solution.mineProbs.end(), remove),
        solution.mineProbs.end());

    solution.outsideMineProb /= totalWeight;
    for (auto& mineProb : solution.mineProbs)
        mineProb.prob /= totalWeight;

    return {solution};
}

}
