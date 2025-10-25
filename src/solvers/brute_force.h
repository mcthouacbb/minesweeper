#pragma once

#include "../board_image.h"
#include "../util/bitset.h"
#include "solution_info.h"

#include <bit>
#include <unordered_map>

namespace solvers::brute_force
{

// a constraint represents a single numbered cell
struct Constraint
{
    uint64_t mask;
    uint32_t sum;

    void addIndex(uint32_t idx)
    {
        mask |= 1ull << idx;
    }
};

inline std::optional<SolutionInfo> solve(const BoardImage& image)
{
    constexpr uint32_t MAX_UNCLEARED = 35;

    // bookkeeping information
    // each unknown cell is assigned an index
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

    // avoid trying to brute force with too large of a state space
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
        - image.zeroCells().size() - image.numberedCells().size();
    for (uint64_t mines = 0; mines < 1ull << uncleared.size(); mines++)
    {
        bool valid = true;
        for (const auto& constraint : constraints)
        {
            // skip this configuration if it does not match the constraints
            if (std::popcount(constraint.mask & mines) != constraint.sum)
            {
                valid = false;
                break;
            }
        }
        if (!valid)
            continue;

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
            if (outsideMineCells + 1 + i - image.numMines() <= 0)
                continue;
            currWeight *= static_cast<double>(image.numMines() - i);
            currWeight /= static_cast<double>(outsideMineCells + 1 + i - image.numMines());
        }

        totalWeight += currWeight;

        uint32_t minesCopy = mines;
        while (minesCopy > 0)
        {
            uint32_t mineIdx = std::countr_zero(minesCopy);
            solution.mineProbs[mineIdx].prob += currWeight;

            minesCopy &= minesCopy - 1;
        }
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

    const auto remove = [&](const SolutionInfo::MineProb& mineProb)
    {
        return solution.isClear(mineProb.point) || solution.isMine(mineProb.point);
    };
    solution.mineProbs.erase(
        std::remove_if(solution.mineProbs.begin(), solution.mineProbs.end(), remove),
        solution.mineProbs.end());

    for (auto& mineProb : solution.mineProbs)
        mineProb.prob /= totalWeight;

    return {solution};
}

}
