#include "board.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <iterator>

Board::Board(uint32_t width, uint32_t height)
	: m_Cells(width * height, CellState::UNCLEARED), m_Width(width), m_Height(height), m_NumMines(0)
{

}

void Board::genMines(uint32_t numMines)
{
	static std::mt19937 rng{384747};

	std::vector<uint32_t> nums(m_Width * m_Height);
	std::iota(nums.begin(), nums.end(), 0);
	std::vector<uint32_t> result;
	std::sample(nums.begin(), nums.end(), std::back_inserter(result), numMines, rng);

	std::fill(m_Cells.begin(), m_Cells.end(), CellState::UNCLEARED);
	for (uint32_t i : result)
		m_Cells[i] = CellState::MINE;
	m_NumMines = numMines;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
	for (uint32_t y = 0; y < board.m_Height; y++)
	{
		for (uint32_t x = 0; x < board.m_Width; x++)
		{
			switch (board.m_Cells[x + y * board.m_Width])
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
	os << "Board size: " << board.m_Width << " x " << board.m_Height << "\n";
	os << "Number of mines: " << board.m_NumMines << "\n";
	return os;
}
