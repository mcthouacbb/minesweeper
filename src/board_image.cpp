#include "board_image.h"

#include <sstream>

BoardImage::BoardImage(const BoardData& data)
    : m_Data(data)
{
}

bool BoardImage::cellCleared(Point pt) const
{
    for (auto cell : m_NumberedCells)
        if (cell.location == pt)
            return true;
    for (auto cell : m_ZeroCells)
        if (cell == pt)
            return true;
    return false;
}

// assumes the cell is clear
int BoardImage::cellNumber(Point pt) const
{
    for (auto cell : m_NumberedCells)
        if (cell.location == pt)
            return cell.adjacentMines;
    return 0;
}

std::ostream& operator<<(std::ostream& os, const BoardImage& boardImage)
{
    for (uint32_t y = 0; y < boardImage.height(); y++)
    {
        for (uint32_t x = 0; x < boardImage.width(); x++)
        {
            if (boardImage.cellCleared({x, y}))
            {
                int adjacent = boardImage.cellNumber({x, y});
                if (adjacent > 0)
                    os << adjacent;
                else
                    os << ' ';
            }
            else
                os << 'O';
        }
        os << '\n';
    }
    os << "Board Image size: " << boardImage.width() << " x " << boardImage.height() << "\n";
    os << "Number of mines: " << boardImage.numMines() << "\n";
    return os;
}

std::string BoardImage::renderSolution(const SolutionInfo& solution) const
{
    std::ostringstream oss;
    for (uint32_t y = 0; y < height(); y++)
    {
        for (uint32_t x = 0; x < width(); x++)
        {
            if (cellCleared({x, y}))
            {
                int adjacent = cellNumber({x, y});
                if (adjacent > 0)
                    oss << adjacent;
                else
                    oss << ' ';
            }
            else if (solution.isMine({x, y}))
            {
                oss << "\033[31mO\033[0m";
            }
            else if (solution.isClear({x, y}))
            {
                oss << "\033[94mO\033[0m";
            }
            else
            {
                oss << 'O';
            }
        }
        oss << '\n';
    }
    oss << "Solution Render size: " << width() << " x " << height() << "\n";
    oss << "Number of counted solutions: " << solution.numValidSolutions << "\n";
    oss << "Total number of mines: " << numMines() << "\n";
    oss << "    Known mines: " << solution.mines.size() << "\n";
    oss << "    Unknown mines: " << numMines() - solution.mines.size() << "\n";

    return oss.str();
}

BoardImageBuilder::BoardImageBuilder(const BoardData& data)
    : m_Data(data)
{
}

void BoardImageBuilder::addClearedCell(Point location, uint32_t adjacentMines)
{
    m_ClearedCells.insert({location, adjacentMines});
}

BoardImage BoardImageBuilder::build() const
{
    constexpr std::pair<int, int> neighborOffsets[] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};

    BoardImage result(m_Data);

    for (const auto& [location, adjacentMines] : m_ClearedCells)
    {
        if (adjacentMines == 0)
            result.addZeroCell(location);
        else
        {
            CellInfo cellInfo = {};
            cellInfo.location = location;
            cellInfo.adjacentMines = adjacentMines;

            for (const auto offset : neighborOffsets)
            {
                Point pt{offset.first + location.x, offset.second + location.y};
                if (pt.x < 0 || pt.x >= m_Data.width || pt.y < 0 || pt.y >= m_Data.height)
                    continue;
                if (m_ClearedCells.count(pt) == 0)
                    cellInfo.unclearedNeighbors.push_back(pt);
            }

            result.addNumberedCell(cellInfo);
        }
    }
    return result;
}
