#include "board.h"
#include "solvers/brute_force.h"
#include <iostream>

void tryRenderSolution(const BoardImage& image)
{
    if (auto solution = solvers::brute_force::solve(image))
        std::cout << image.renderSolution(solution.value()) << std::endl;
    else
        std::cout << "No solution available" << std::endl;
}

int main()
{
    Board board(20, 10);
    board.genMines(40);
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    tryRenderSolution(board.genImage());

    board.makeMove({7, 6});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    tryRenderSolution(board.genImage());

    board.makeMove({4, 4});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    tryRenderSolution(board.genImage());

    board.makeMove({11, 9});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    tryRenderSolution(board.genImage());

    return 0;
}
