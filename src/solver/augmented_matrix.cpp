#include "augmented_matrix.h"
#include <algorithm>
#include <stdexcept>
#include <iomanip>

AugmentedMatrix::AugmentedMatrix(uint32_t width, uint32_t height)
	: m_Elements((width + 1) * height), m_Width(width), m_Height(height)
{

}

void AugmentedMatrix::swap(const RowRef& a, const RowRef& b)
{
	std::swap_ranges(&a[0], &a[0] + width() + 1, &b[0]);
}

AugmentedMatrix::RowRef AugmentedMatrix::operator[](size_t idx)
{
	return RowRef{*this, idx};
}

AugmentedMatrix::ConstRowRef AugmentedMatrix::operator[](size_t idx) const
{
	return ConstRowRef{*this, idx};
}

void AugmentedMatrix::solve()
{
	int currRow = 0;
	for (int currCol = 0; currCol < width(); currCol++)
	{
		int row = currRow;
		for (; row < height(); row++)
		{
			if ((*this)[row][currCol] != 0)
				break;
		}
		if (row == height())
			continue;

		if (row != currRow)
			swap((*this)[row], (*this)[currRow]);

		int pivot = (*this)[currRow][currCol];
		if (pivot != 1)
		{
			for (int j = currCol; j <= width(); j++)
			{
				int& elem = (*this)[currRow][j];
				if (elem % pivot != 0)
					throw std::runtime_error("Row was not divisible by pivot");
				elem /= pivot;
			}
		}

		for (int j = 0; j < height(); j++)
		{
			if (j == currRow)
				continue;
			int factor = (*this)[j][currCol];
			if (factor == 0)
				continue;
			for (int k = currCol; k <= width(); k++)
				(*this)[j][k] -= factor * (*this)[currRow][k];
		}

		currRow++;
	}
}

std::ostream& operator<<(std::ostream& os, const AugmentedMatrix& matrix)
{
	for (int row = 0; row < matrix.height(); row++)
	{
		os << "[";
		for (int col = 0; col < matrix.width(); col++)
		{
			os << std::setw(4) << matrix[row][col];
			if (col != matrix.width() - 1)
				os << ", ";
		}
		os << " | " << std::setw(4) << matrix[row][matrix.width()] << "]\n";
	}
	return os;
}
