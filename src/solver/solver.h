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
	void extractInfo(const LinearSystem& solved);

	const BoardImage& m_Image;
	VarTable m_VarTable;
};
