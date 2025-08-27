#include "board.h"
#include "solvers/brute_force.h"
#include <iostream>

int main()
{
    Board board(20, 10);
    board.genMines(40);
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    solvers::brute_force::solve(board.genImage());

    board.makeMove({7, 6});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    solvers::brute_force::solve(board.genImage());

    board.makeMove({4, 4});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    solvers::brute_force::solve(board.genImage());

    board.makeMove({11, 9});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    solvers::brute_force::solve(board.genImage());
    return 0;
}
