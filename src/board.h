#pragma once

#include <vector>
#include <cstdint>
#include <ostream>
#include "board_image.h"

enum class CellState : uint8_t
{
	MINE,
	UNCLEARED,
	CLEARED
};

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

