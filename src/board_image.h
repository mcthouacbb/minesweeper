#pragma once

#include "solvers/solution_info.h"
#include "types.h"
#include "util/static_vector.h"
#include <ostream>
#include <unordered_map>
#include <vector>

struct CellInfo
{
    Point location;
    uint32_t adjacentMines;
    StaticVector<Point, 8> unclearedNeighbors;
};

class BoardImage
{
public:
    BoardImage(const BoardData& data);

    void addNumberedCell(CellInfo cell);
    void addZeroCell(Point location);

    bool cellCleared(Point pt) const;
    // assumes the cell is clear
    int cellNumber(Point pt) const;
    uint32_t width() const;
    uint32_t height() const;
    uint32_t numMines() const;

    const std::vector<CellInfo>& numberedCells() const;
    const std::vector<Point>& zeroCells() const;

    std::string renderSolution(const SolutionInfo& solution) const;

private:
    BoardData m_Data;
    std::vector<CellInfo> m_NumberedCells;
    std::vector<Point> m_ZeroCells;
};

std::ostream& operator<<(std::ostream& os, const BoardImage& image);

inline void BoardImage::addNumberedCell(CellInfo cell)
{
    m_NumberedCells.push_back(cell);
}

inline void BoardImage::addZeroCell(Point location)
{
    m_ZeroCells.push_back(location);
}

inline uint32_t BoardImage::width() const
{
    return m_Data.width;
}

inline uint32_t BoardImage::height() const
{
    return m_Data.height;
}

inline uint32_t BoardImage::numMines() const
{
    return m_Data.numMines;
}

inline const std::vector<CellInfo>& BoardImage::numberedCells() const
{
    return m_NumberedCells;
}

inline const std::vector<Point>& BoardImage::zeroCells() const
{
    return m_ZeroCells;
}

class BoardImageBuilder
{
public:
    BoardImageBuilder(const BoardData& data);

    void addClearedCell(Point location, uint32_t adjacentMines);

    BoardImage build() const;

private:
    BoardData m_Data;
    std::unordered_map<Point, uint32_t, PointHash> m_ClearedCells;
};
