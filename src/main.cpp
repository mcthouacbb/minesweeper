#include "board.h"
#include "solvers/basic_optimized.h"
#include "solvers/brute_force.h"
#include "test_suite.h"
#include "test_suite_gen.h"
#include <iomanip>
#include <iostream>
#include <sstream>

void tryRenderSolution(const BoardImage& image)
{
    if (auto solution = solvers::basic_optimized::solve(image))
        std::cout << image.renderSolution(solution.value()) << std::endl;
    else
        std::cout << "No solution available" << std::endl;
}

enum class GameResult
{
    WIN,
    LOSS,
    TOO_COMPLEX,
    UNSOLVABLE
};

GameResult simulateGameDeterministic(uint32_t width, uint32_t height, uint32_t numMines)
{
    static std::random_device rd;
    static std::mt19937 gen{rd()};
    Board board{width, height};

    MoveResult moveResult;
    do
    {
        board.genMines(numMines);
        std::uniform_int_distribution<uint32_t> distW(0, width - 1);
        std::uniform_int_distribution<uint32_t> distH(0, height - 1);
        moveResult = board.makeMove(Point{distW(gen), distH(gen)});
    } while (moveResult == MoveResult::MINE);

    while (board.numCleared() < board.width() * board.height() - board.numMines())
    {
        BoardImage image = board.genImage();
        auto solution = solvers::basic_optimized::solve(image);
        if (!solution.has_value())
            return GameResult::TOO_COMPLEX;
        if (solution->clears.empty())
            return GameResult::UNSOLVABLE;

        for (Point clear : solution->clears)
        {
            MoveResult moveResult = board.makeMove(clear);
            if (moveResult == MoveResult::MINE)
                return GameResult::LOSS;
        }
    }

    return GameResult::WIN;
}

GameResult simulateGameProbabilistic(uint32_t width, uint32_t height, uint32_t numMines)
{
    // static std::random_device rd;
    static std::mt19937 gen{293483 /*rd()*/};
    Board board{width, height};

    MoveResult moveResult;
    do
    {
        board.genMines(numMines);
        std::uniform_int_distribution<uint32_t> distW(0, width - 1);
        std::uniform_int_distribution<uint32_t> distH(0, height - 1);
        moveResult = board.makeMove(Point{distW(gen), distH(gen)});
    } while (moveResult == MoveResult::MINE);

    while (board.numCleared() < board.width() * board.height() - board.numMines())
    {
        BoardImage image = board.genImage();
        auto solution = solvers::basic_optimized::solve(image);
        if (!solution.has_value())
            return GameResult::TOO_COMPLEX;
        if (solution->clears.empty())
        {
            double lowestProb = 2.0;
            Point bestPoint = {};
            for (const auto& mineProb : solution->mineProbs)
            {
                if (mineProb.prob < lowestProb)
                {
                    lowestProb = mineProb.prob;
                    bestPoint = mineProb.point;
                }
            }
            MoveResult moveResult = board.makeMove(bestPoint);
            if (moveResult == MoveResult::MINE)
                return GameResult::LOSS;
            if (moveResult == MoveResult::ILLEGAL)
            {
                return GameResult::LOSS;
            }
        }

        for (Point clear : solution->clears)
        {
            MoveResult moveResult = board.makeMove(clear);
            if (moveResult == MoveResult::MINE)
                return GameResult::LOSS;
        }
    }

    return GameResult::WIN;
}

std::string toPercentStats(uint32_t success, uint32_t total)
{
    double frac = static_cast<double>(success) / static_cast<double>(total);
    double stdev = std::sqrt(frac * (1.0 - frac) / total);
    double errorBar = 2 * stdev;
    std::ostringstream result;
    result << std::fixed << std::setprecision(2);
    result << frac * 100 << "% +/- " << errorBar * 100 << "%";
    return result.str();
}

void simulateGamesDeterministic(uint32_t width, uint32_t height, uint32_t numMines, uint32_t games = 100)
{
    std::array<uint32_t, 4> results = {};
    for (uint32_t simul = 0; simul < games; simul++)
    {
        GameResult result = simulateGameDeterministic(width, height, numMines);
        results[static_cast<uint32_t>(result)]++;
    }
    std::cout << "WIDTH: " << width << " HEIGHT: " << height << " MINES: " << numMines
              << " SOLVED: " << toPercentStats(results[0], games)
              << " TOO_COMPLEX: " << toPercentStats(results[2], games)
              << " UNSOLVABLE: " << toPercentStats(results[3], games) << std::endl;
}

void simulateGamesProbabilistic(uint32_t width, uint32_t height, uint32_t numMines, uint32_t games = 100)
{
    std::array<uint32_t, 4> results = {};
    for (uint32_t simul = 0; simul < games; simul++)
    {
        GameResult result = simulateGameProbabilistic(width, height, numMines);
        results[static_cast<uint32_t>(result)]++;
    }
    std::cout << "WIDTH: " << width << " HEIGHT: " << height << " MINES: " << numMines
              << " SOLVED: " << toPercentStats(results[0], games)
              << " TOO_COMPLEX: " << toPercentStats(results[2], games)
              << " UNSOLVABLE: " << toPercentStats(results[3], games) << std::endl;
}

int main()
{
    // simulateGamesDeterministic(9, 9, 10, 5000);
    // simulateGamesDeterministic(16, 16, 40, 5000);
    // simulateGamesDeterministic(30, 16, 99, 5000);

    simulateGamesProbabilistic(9, 9, 10, 10);
    simulateGamesProbabilistic(16, 16, 40, 10);
    // simulateGamesProbabilistic(30, 16, 99, 5000);

    // BoardImageBuilder builder({15, 15, 35});
    // builder.addClearedCell({3, 3}, 0);
    // builder.addClearedCell({4, 3}, 0);
    // builder.addClearedCell({3, 4}, 0);
    // builder.addClearedCell({4, 4}, 0);

    // builder.addClearedCell({2, 2}, 2);
    // builder.addClearedCell({3, 2}, 1);
    // builder.addClearedCell({4, 2}, 1);
    // builder.addClearedCell({5, 2}, 2);

    // builder.addClearedCell({2, 3}, 1);
    // builder.addClearedCell({5, 3}, 2);

    // builder.addClearedCell({2, 4}, 1);
    // builder.addClearedCell({5, 4}, 2);

    // builder.addClearedCell({2, 5}, 2);
    // builder.addClearedCell({3, 5}, 1);
    // builder.addClearedCell({4, 5}, 1);
    // builder.addClearedCell({5, 5}, 2);

    // builder.addClearedCell({5, 6}, 1);
    // builder.addClearedCell({6, 6}, 2);
    // builder.addClearedCell({5, 7}, 2);
    // builder.addClearedCell({4, 8}, 2);

    // tryRenderSolution(builder.build());

    // std::cout << builder.build() << std::endl;

    // BoardImageBuilder builder2({8, 6, 8});
    // builder2.addClearedCell({2, 2}, 3);
    // builder2.addClearedCell({4, 3}, 2);
    // builder2.addClearedCell({1, 5}, 1);

    // tryRenderSolution(builder2.build());
    // std::cout << builder2.build() << std::endl;

    // regenerateTestSuite();

    // run_test_suite(TestSuite::EASY, solvers::basic_optimized::solve);
    // run_test_suite(TestSuite::MEDIUM, solvers::basic_optimized::solve);
    // run_test_suite(TestSuite::HARD, solvers::basic_optimized::solve);

    // run_test_suite(TestSuite::EASY, solvers::brute_force::solve);
    // run_test_suite(TestSuite::MEDIUM, solvers::brute_force::solve);
    // run_test_suite(TestSuite::HARD, solvers::brute_force::solve);

    return 0;
}
