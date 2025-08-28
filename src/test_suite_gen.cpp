#include "board.h"
#include "solvers/brute_force.h"
#include "test_suite.h"

#include <chrono>
#include <random>
#include <string>

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

std::string getTestPosStr(const BoardImage& image, const SolutionInfo& solutionInfo)
{
    std::string result = "    {\n";
    result += "        {";
    result += std::to_string(image.width()) + ", ";
    result += std::to_string(image.height()) + ", ";
    result += std::to_string(image.numMines()) + "},\n";

    result += "        {";
    bool first = true;
    for (Point zeroCell : image.zeroCells())
    {
        if (!first)
            result += ", ";
        result += "{{";
        result += std::to_string(zeroCell.x) + ", ";
        result += std::to_string(zeroCell.y) + "}, 0}";
        first = false;
    }

    for (CellInfo numberedCell : image.numberedCells())
    {
        if (!first)
            result += ", ";
        result += "{{";
        result += std::to_string(numberedCell.location.x) + ", ";
        result += std::to_string(numberedCell.location.y) + "}, ";
        result += std::to_string(numberedCell.adjacentMines) + "}";
        first = false;
    }
    result += "},\n";

    result += "        {\n";
    result += "            {";
    first = true;
    for (Point mine : solutionInfo.mines)
    {
        if (!first)
            result += ", ";
        result += "{";
        result += std::to_string(mine.x) + ", ";
        result += std::to_string(mine.y) + "}";
        first = false;
    }
    result += "},\n";
    result += "            {";
    first = true;
    for (Point clear : solutionInfo.clears)
    {
        if (!first)
            result += ", ";
        result += "{";
        result += std::to_string(clear.x) + ", ";
        result += std::to_string(clear.y) + "}";
        first = false;
    }
    result += "},\n";
    result += "            " + std::to_string(solutionInfo.numValidSolutions) + "\n        }\n";
    result += "    }";

    return result;
}

void generateTestSuite()
{
    std::random_device rd;
    auto seed = rd();
    std::mt19937 gen(seed);
    std::cout << "Seed for generating test suite: " << seed << std::endl;
    // std::mt19937 gen(283473842);
    std::string easyCases = "std::vector<TestPosition> easyCases = {";
    std::string mediumCases = "std::vector<TestPosition> mediumCases = {";
    std::string hardCases = "std::vector<TestPosition> hardCases = {";

    for (uint32_t i = 0; i < 100; i++)
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

        std::string& result = [&]() -> std::string&
        {
            if (seconds < 0.1)
            {
                std::cout << "easy solve" << std::endl;
                return easyCases;
            }
            else if (seconds < 1)
            {
                std::cout << "medium solve" << std::endl;
                return mediumCases;
            }
            else
            {
                std::cout << "hard solve" << std::endl;
                return hardCases;
            }
        }();

        if (i > 0)
            result += ",";
        result += "\n" + getTestPosStr(image, solution.value());

        std::cout << std::endl;
    }
    easyCases += "\n};\n";
    mediumCases += "\n};\n";
    hardCases += "\n};\n";
    std::cout << easyCases << std::endl;
    std::cout << mediumCases << std::endl;
    std::cout << hardCases << std::endl;
}
