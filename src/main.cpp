#include "board.h"
#include "solvers/brute_force.h"
#include <iostream>

int main()
{
    Board board(20, 10);
    board.genMines(40);
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    std::cout << board.genImage().renderSolution(solvers::brute_force::solve(board.genImage()))
              << std::endl;

    board.makeMove({7, 6});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    std::cout << board.genImage().renderSolution(solvers::brute_force::solve(board.genImage()))
              << std::endl;

    board.makeMove({4, 4});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    std::cout << board.genImage().renderSolution(solvers::brute_force::solve(board.genImage()))
              << std::endl;

    board.makeMove({11, 9});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    std::cout << board.genImage().renderSolution(solvers::brute_force::solve(board.genImage()))
              << std::endl;

    return 0;
}
