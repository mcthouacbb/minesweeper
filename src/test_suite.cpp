#include "test_suite.h"
#include "raw_test_data.h"

#include <chrono>
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

void run_test_suite(TestSuite suite, Solver solver, bool verbose)
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

    auto t1 = std::chrono::steady_clock::now();

    uint32_t numPassed = 0;
    uint32_t numFailed = 0;

    for (const auto& pos : positions)
    {
        BoardImageBuilder builder(pos.data);
        for (const auto& [location, adjacentMines] : pos.clearedCells)
            builder.addClearedCell(location, adjacentMines);

        BoardImage image = builder.build();
        SolutionInfo solution = solver(image).value();

        if (verbose)
            std::cout << image.renderSolution(solution) << std::endl;

        bool passed = true;

        if (pos.solutionInfo.numValidSolutions != solution.numValidSolutions)
        {
            passed = false;
            std::cout << "Failed: number of valid mine configurations does not match" << std::endl;
            std::cout << "    Expected: " << pos.solutionInfo.numValidSolutions << std::endl;
            std::cout << "    Computed: " << solution.numValidSolutions << std::endl;
        }

        using PointSet = std::unordered_set<Point, PointHash>;

        if (PointSet(pos.solutionInfo.mines.begin(), pos.solutionInfo.mines.end())
            != PointSet(solution.mines.begin(), solution.mines.end()))
        {
            passed = false;
            std::cout << "Failed: did not correctly identify which squares were always mines"
                      << std::endl;
        }

        if (PointSet(pos.solutionInfo.clears.begin(), pos.solutionInfo.clears.end())
            != PointSet(solution.clears.begin(), solution.clears.end()))
        {
            passed = false;
            std::cout << "Failed: did not correctly identify which squares were always clear"
                      << std::endl;
        }

        numPassed += passed;
        numFailed += !passed;
    }

    auto t2 = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
    switch (suite)
    {
        case TestSuite::EASY:
            std::cout << "Finished easy test suite" << std::endl;
            break;
        case TestSuite::MEDIUM:
            std::cout << "Finished medium test suite" << std::endl;
            break;
        case TestSuite::HARD:
            std::cout << "Finished hard test suite" << std::endl;
            break;
    }
    std::cout << "Test suite took " << seconds << " seconds. " << positions.size() / seconds
              << " cases/sec" << std::endl;
    std::cout << "Passed: " << numPassed << "/" << (numPassed + numFailed) << std::endl;
    std::cout << "Failed: " << numFailed << "/" << (numPassed + numFailed) << std::endl;
}
