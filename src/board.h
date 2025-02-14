#include <vector>
#include <cstdint>
#include <ostream>
#include "util/static_vector.h"

enum class CellState : uint8_t
{
	MINE,
	UNCLEARED,
	CLEARED
};

struct Point
{
	uint32_t x;
	uint32_t y;

	constexpr bool operator==(const Point& other) const noexcept = default;
	constexpr bool operator!=(const Point& other) const noexcept = default;
};

struct BoardData
{
	uint32_t width;
	uint32_t height;
	uint32_t numMines;
};

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
private:
	BoardData m_Data;
	std::vector<CellInfo> m_NumberedCells;
	std::vector<Point> m_ZeroCells;
};

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

class Board
{
public:
	Board(uint32_t width, uint32_t height);

	void genMines(uint32_t numMines);

	CellState cell(Point pt) const;
	uint32_t width() const;
	uint32_t height() const;
	uint32_t numMines() const;
	BoardImage genImage();
private:
	BoardData m_Data;
	std::vector<CellState> m_Cells;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
std::ostream& operator<<(std::ostream& os, const BoardImage& image);

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

