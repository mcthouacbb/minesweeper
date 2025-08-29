#pragma once

#include "board_image.h"
#include "solvers/solution_info.h"

struct TestPosition
{
    BoardData data;
    std::vector<std::pair<Point, uint32_t>> clearedCells;
    SolutionInfo solutionInfo;

    static TestPosition fromImage(const BoardImage& image, const SolutionInfo& solution);
};

enum class TestSuite
{
    EASY,
    MEDIUM,
    HARD
};

void run_test_suite(TestSuite suite, Solver solver, bool verbose = false);
