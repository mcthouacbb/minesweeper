#include <vector>
#include <cstdint>
#include <ostream>

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

	friend std::ostream& operator<<(std::ostream& os, const Board& board);
private:
	std::vector<CellState> m_Cells;
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_NumMines;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
