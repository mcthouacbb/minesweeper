#include "board.h"
#include "solvers/brute_force.h"

#include <chrono>
#include <random>

void clearRandomCell(Board& board, std::mt19937& gen)
{
    std::uniform_int_distribution<uint32_t> distX(0, board.width() - 1);
    std::uniform_int_distribution<uint32_t> distY(0, board.height() - 1);

    MoveResult result;
    do
    {
        result = board.makeMove({distX(gen), distY(gen)});
    } while (result != MoveResult::CLEAR);
}

void generateTestSuite()
{
    std::mt19937 gen(283473842);
    for (uint32_t i = 0; i < 15; i++)
    {
        Board board{15, 15};
        board.genMines(40, gen);
        do
        {
            clearRandomCell(board, gen);
        } while (board.numCleared() < 12);

        BoardImage image = board.genImage();

        auto t1 = std::chrono::steady_clock::now();
        auto solution = solvers::brute_force::solve(image);
        auto t2 = std::chrono::steady_clock::now();

        if (!solution.has_value())
        {
            // std::cout << image;
            std::cout << "Solution will take too long, skipping" << std::endl;
            i--;
            continue;
        }
        else
        {
            std::cout << image.renderSolution(solution.value());
        }

        double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
        std::cout << "Took " << seconds << " seconds to find the solution: ";
        if (seconds < 0.01)
            std::cout << "very easy solve" << std::endl;
        else if (seconds < 0.1)
            std::cout << "easy solve" << std::endl;
        else if (seconds < 1)
            std::cout << "medium solve" << std::endl;
        else
            std::cout << "hard solve" << std::endl;

        std::cout << std::endl;
    }
}
