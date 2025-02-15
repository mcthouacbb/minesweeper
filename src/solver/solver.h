#pragma once

#include "../board_image.h"
#include "system_builder.h"

class Solver
{
public:
	Solver(const BoardImage& image);

	void solve();
private:
	const BoardImage& m_Image;

};

inline void Solver::solve()
{
	LinearSystemBuilder builder;
	
}
