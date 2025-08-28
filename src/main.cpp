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

    // board.makeMove({11, 9});
    // std::cout << board << std::endl;
    // std::cout << board.genImage() << std::endl;

    // tryRenderSolution(board.genImage());

    BoardImageBuilder builder({15, 15, 35});
    builder.addClearedCell({3, 3}, 0);
    builder.addClearedCell({4, 3}, 0);
    builder.addClearedCell({3, 4}, 0);
    builder.addClearedCell({4, 4}, 0);

    builder.addClearedCell({2, 2}, 2);
    builder.addClearedCell({3, 2}, 1);
    builder.addClearedCell({4, 2}, 1);
    builder.addClearedCell({5, 2}, 2);

    builder.addClearedCell({2, 3}, 1);
    builder.addClearedCell({5, 3}, 2);

    builder.addClearedCell({2, 4}, 1);
    builder.addClearedCell({5, 4}, 2);

    builder.addClearedCell({2, 5}, 2);
    builder.addClearedCell({3, 5}, 1);
    builder.addClearedCell({4, 5}, 1);
    builder.addClearedCell({5, 5}, 2);

    builder.addClearedCell({5, 6}, 1);
    builder.addClearedCell({6, 6}, 2);
    builder.addClearedCell({5, 7}, 2);
    builder.addClearedCell({4, 8}, 2);

    tryRenderSolution(builder.build());

    std::cout << builder.build() << std::endl;

    BoardImageBuilder builder2({8, 6, 8});
    builder2.addClearedCell({2, 2}, 3);
    builder2.addClearedCell({4, 3}, 2);
    builder2.addClearedCell({1, 5}, 1);

    tryRenderSolution(builder2.build());
    std::cout << builder2.build() << std::endl;

    return 0;
}
