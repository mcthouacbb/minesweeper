#include "board_image.h"

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