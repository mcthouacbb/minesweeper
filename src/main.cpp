#include <iostream>
#include "board.h"
#include "solver/augmented_matrix.h"

int main()
{
    AugmentedMatrix matrix{5, 5};
    matrix[0][0] = 1;
    matrix[0][1] = 1;
    matrix[0][5] = 1;
    matrix[1][0] = matrix[1][1] = matrix[1][2] = 1;
    matrix[1][5] = 2;
    matrix[2][3] = matrix[2][1] = matrix[2][2] = 1;
    matrix[2][5] = 2;
    matrix[3][3] = matrix[3][4] = matrix[3][2] = 1;
    matrix[3][5] = 2;
    matrix[4][3] = matrix[4][4] = 1;
    matrix[4][5] = 1;
    std::cout << matrix << std::endl;
    matrix.solve();
    std::cout << matrix << std::endl;
    /*Board board(20, 10);
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
    std::cout << board.genImage() << std::endl;*/
    return 0;
}
