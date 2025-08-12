#include "solver.h"

Solver::Solver(const BoardImage& image)
	: m_Image(image)
{

}

void Solver::solve()
{
	LinearSystem topLevelSystem = {};
	for (auto& cell : m_Image.numberedCells())
	{
		auto& equation = topLevelSystem.addEquation();
		for (auto neighbor : cell.unclearedNeighbors)
			equation.addTerm(1, m_VarTable.varID(neighbor));
		equation.setRHS(cell.adjacentMines);
	}

	AugmentedMatrix matrix = topLevelSystem.toAugmentedMatrix(m_VarTable.size());
	std::cout << matrix << std::endl;
	matrix.solve();
	std::cout << matrix << std::endl;
	LinearSystem solved(matrix);
	extractInfo(solved);
}

void Solver::extractInfo(const LinearSystem& solved)
{
	std::vector<int> varValues(m_VarTable.size(), -1);
	for (const auto& equation : solved.equations())
	{
		int lowerBound = 0;
		int upperBound = 0;
		for (const auto& term : equation.lhs)
		{
			if (varValues[term.varID] != -1)
			{
				lowerBound += term.coeff * varValues[term.varID];
				upperBound += term.coeff * varValues[term.varID];
				continue;
			}
			if (term.coeff > 0)
				upperBound += term.coeff;
			else
				lowerBound += term.coeff;
		}

		if (equation.rhs == upperBound)
		{
			for (const auto& term : equation.lhs)
			{
				if (varValues[term.varID] != -1)
					continue;
				if (term.coeff > 0)
					varValues[term.varID] = 1;
				else
					varValues[term.varID] = 0;
			}
		}
		else if (equation.rhs == lowerBound)
		{
			for (const auto& term : equation.lhs)
			{
				if (varValues[term.varID] != -1)
					continue;
				if (term.coeff > 0)
					varValues[term.varID] = 0;
				else
					varValues[term.varID] = 1;
			}
		}
	}
}
