#include <iostream>
#include "board.h"

int main()
{
    Board board(20, 10);
    board.genMines(40);
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    board.makeMove({7, 6});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    board.makeMove({4, 4});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;

    board.makeMove({11, 9});
    std::cout << board << std::endl;
    std::cout << board.genImage() << std::endl;
    return 0;
}
