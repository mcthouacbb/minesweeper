#include <iostream>
#include "board.h"

int main()
{
    Board board(10, 5);
    board.genMines(20);
    std::cout << board << std::endl;
    return 0;
}
