#pragma once

#include "../board_image.h"
#include "linear_system.h"
#include <iostream>
#include <unordered_map>

struct VarTable
{
	std::unordered_map<Point, uint32_t, PointHash> varToID;
	std::vector<Point> idToVar;

	uint32_t varID(Point pt)
	{
		if (varToID.contains(pt))
			return varToID.at(pt);
		uint32_t result = varToID[pt] = idToVar.size();
		idToVar.push_back(pt);
		return result;
	}

	uint32_t size()
	{
		return idToVar.size();
	}
};

class Solver
{
public:
	Solver(const BoardImage& image);

	void solve();
private:
	const BoardImage& m_Image;

};

inline Solver::Solver(const BoardImage& image)
	: m_Image(image)
{

}

inline void Solver::solve()
{
	VarTable varTable;
	LinearSystem topLevelSystem = {};
	for (auto& cell : m_Image.numberedCells())
	{
		auto& equation = topLevelSystem.addEquation();
		for (auto neighbor : cell.unclearedNeighbors)
			equation.addTerm(1, varTable.varID(neighbor));
		equation.setRHS(cell.adjacentMines);
	}

	AugmentedMatrix matrix = topLevelSystem.toAugmentedMatrix(varTable.size());
	std::cout << matrix << std::endl;
	matrix.solve();
	std::cout << matrix << std::endl;
	LinearSystem solved(matrix);
	
}
