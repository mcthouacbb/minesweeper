#include "board.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <iterator>

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

Board::Board(uint32_t width, uint32_t height)
	: m_Data{width, height}, m_Cells(width * height, CellState::UNCLEARED)
{

}

void Board::genMines(uint32_t numMines)
{
	static std::mt19937 rng{384747};

	std::vector<uint32_t> nums(width() * height());
	std::iota(nums.begin(), nums.end(), 0);
	std::vector<uint32_t> result;
	std::sample(nums.begin(), nums.end(), std::back_inserter(result), numMines, rng);

	std::fill(m_Cells.begin(), m_Cells.end(), CellState::UNCLEARED);
	for (uint32_t i : result)
		m_Cells[i] = CellState::MINE;
	m_Data.numMines = numMines;
}

BoardImage Board::genImage()
{
	constexpr std::pair<int, int> offsets[] = {
		{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}
	};
	BoardImage result{m_Data};
	for (uint32_t y = 0; y < height(); y++)
	{
		for (uint32_t x = 0; x < width(); x++)
		{
			if (cell({x, y}) != CellState::CLEARED)
				continue;
			CellInfo cellInfo = {};
			cellInfo.location = {x, y};
			for (auto offset : offsets)
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
