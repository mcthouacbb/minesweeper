#pragma once

#include "board_image.h"
#include <cstdint>
#include <ostream>
#include <random>
#include <vector>

enum class CellState : uint8_t
{
    MINE,
    UNCLEARED,
    CLEARED
};

enum class MoveResult
{
    ILLEGAL,
    CLEAR,
    MINE
};

class Board
{
public:
    Board(uint32_t width, uint32_t height);

    void genMines(uint32_t numMines, std::mt19937& gen);
    void genMines(uint32_t numMines);
    MoveResult makeMove(Point move);

    CellState cell(Point pt) const;
    uint32_t width() const;
    uint32_t height() const;
    uint32_t numMines() const;
    uint32_t numCleared() const;
    BoardImage genImage() const;

private:
    void clearCells(Point location);

    BoardData m_Data;
    std::vector<CellState> m_Cells;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

inline CellState Board::cell(Point pt) const
{
    return m_Cells[pt.x + pt.y * width()];
}

inline uint32_t Board::width() const
{
    return m_Data.width;
}

inline uint32_t Board::height() const
{
    return m_Data.height;
}

inline uint32_t Board::numMines() const
{
    return m_Data.numMines;
}

inline uint32_t Board::numCleared() const
{
    uint32_t count = 0;
    for (uint32_t y = 0; y < height(); y++)
    {
        for (uint32_t x = 0; x < width(); x++)
        {
            count += cell({x, y}) == CellState::CLEARED;
        }
    }
    return count;
}
