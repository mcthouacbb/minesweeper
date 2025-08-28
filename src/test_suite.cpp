#include "test_suite.h"
#include "raw_test_data.h"
#include "solvers/brute_force.h"

#include <unordered_set>

TestPosition TestPosition::fromImage(const BoardImage& image, const SolutionInfo& solution)
{
    TestPosition result = {};
    result.data = {image.width(), image.height(), image.numMines()};
    result.solutionInfo = solution;
    for (Point zeroCell : image.zeroCells())
        result.clearedCells.push_back({zeroCell, 0});

    for (auto [location, adjacentMines, unclearedNeighbors] : image.numberedCells())
        result.clearedCells.push_back({location, adjacentMines});

    return result;
}

void run_test_suite(TestSuite suite)
{
    const std::vector<TestPosition>& positions = [&]()
    {
        switch (suite)
        {
            case TestSuite::EASY:
                return easyCases;
            case TestSuite::MEDIUM:
                return mediumCases;
            case TestSuite::HARD:
                return hardCases;
            default:
                throw std::runtime_error("Invalid test case");
        }
    }();

    for (const auto& pos : positions)
    {
        BoardImageBuilder builder(pos.data);
        for (const auto& [location, adjacentMines] : pos.clearedCells)
            builder.addClearedCell(location, adjacentMines);

        BoardImage image = builder.build();
        SolutionInfo solution = solvers::brute_force::solve(image).value();

        std::cout << image.renderSolution(solution) << std::endl;

        if (pos.solutionInfo.numValidSolutions != solution.numValidSolutions)
        {
            std::cout << "Failed: number of valid mine configurations does not match" << std::endl;
            std::cout << "    Expected: " << pos.solutionInfo.numValidSolutions << std::endl;
            std::cout << "    Computed: " << solution.numValidSolutions << std::endl;
        }

        using PointSet = std::unordered_set<Point, PointHash>;

        if (PointSet(pos.solutionInfo.mines.begin(), pos.solutionInfo.mines.end())
            != PointSet(solution.mines.begin(), solution.mines.end()))
        {
            std::cout << "Failed: did not correctly identify which squares were always mines"
                      << std::endl;
        }

        if (PointSet(pos.solutionInfo.clears.begin(), pos.solutionInfo.clears.end())
            != PointSet(solution.clears.begin(), solution.clears.end()))
        {
            std::cout << "Failed: did not correctly identify which squares were always clear"
                      << std::endl;
        }
    }
}
