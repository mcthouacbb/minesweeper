#include "board.h"
#include <algorithm>
#include <iterator>
#include <numeric>

Board::Board(uint32_t width, uint32_t height)
    : m_Data{width, height}, m_Cells(width * height, CellState::UNCLEARED)
{
}

void Board::genMines(uint32_t numMines, std::mt19937& gen)
{
    std::vector<uint32_t> nums(width() * height());
    std::iota(nums.begin(), nums.end(), 0);
    std::vector<uint32_t> result;
    std::sample(nums.begin(), nums.end(), std::back_inserter(result), numMines, gen);

    std::fill(m_Cells.begin(), m_Cells.end(), CellState::UNCLEARED);
    for (uint32_t i : result)
        m_Cells[i] = CellState::MINE;
    m_Data.numMines = numMines;
}

void Board::genMines(uint32_t numMines)
{
    static std::mt19937 rng{384747};
    genMines(numMines, rng);
}

MoveResult Board::makeMove(Point move)
{
    if (cell(move) == CellState::CLEARED)
        return MoveResult::ILLEGAL;
    if (cell(move) == CellState::MINE)
        return MoveResult::MINE;

    clearCells(move);

    return MoveResult::CLEAR;
}

constexpr std::pair<int, int> neighborOffsets[] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

void Board::clearCells(Point location)
{
    if (cell(location) != CellState::UNCLEARED)
        return;

    m_Cells[location.x + width() * location.y] = CellState::CLEARED;

    bool hasAdjacentMine = false;
    for (auto offset : neighborOffsets)
    {
        Point pt{offset.first + location.x, offset.second + location.y};
        if (pt.x < 0 || pt.x >= width() || pt.y < 0 || pt.y >= height())
            continue;
        if (cell(pt) == CellState::MINE)
        {
            hasAdjacentMine = true;
            break;
        }
    }
    if (hasAdjacentMine)
        return;

    for (auto offset : neighborOffsets)
    {
        Point pt{offset.first + location.x, offset.second + location.y};
        if (pt.x < 0 || pt.x >= width() || pt.y < 0 || pt.y >= height())
            continue;
        clearCells(pt);
    }
}

BoardImage Board::genImage() const
{
    BoardImage result{m_Data};
    for (uint32_t y = 0; y < height(); y++)
    {
        for (uint32_t x = 0; x < width(); x++)
        {
            if (cell({x, y}) != CellState::CLEARED)
                continue;
            CellInfo cellInfo = {};
            cellInfo.location = {x, y};
            for (auto offset : neighborOffsets)
            {
                Point pt{offset.first + x, offset.second + y};
                if (pt.x < 0 || pt.x >= width() || pt.y < 0 || pt.y >= height())
                    continue;
                if (cell(pt) == CellState::MINE)
                    cellInfo.adjacentMines++;
                if (cell(pt) != CellState::CLEARED)
                    cellInfo.unclearedNeighbors.push_back(pt);
            }
            if (cellInfo.adjacentMines == 0)
                result.addZeroCell(cellInfo.location);
            else
                result.addNumberedCell(cellInfo);
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    for (uint32_t y = 0; y < board.height(); y++)
    {
        for (uint32_t x = 0; x < board.width(); x++)
        {
            switch (board.cell({x, y}))
            {
                case CellState::MINE:
                    os << 'X';
                    break;
                case CellState::CLEARED:
                    os << '.';
                    break;
                case CellState::UNCLEARED:
                    os << 'O';
                    break;
            }
        }
        os << '\n';
    }
    os << "Board size: " << board.width() << " x " << board.height() << "\n";
    os << "Number of mines: " << board.numMines() << "\n";
    return os;
}
