#pragma once

#include <vector>

// extra column is not counted in the width
class AugmentedMatrix
{
public:
	AugmentedMatrix(uint32_t width, uint32_t height);

	void solve();

	struct RowRef
	{
		AugmentedMatrix& matrix;
		size_t row;

		int& operator[](size_t idx) const
		{
			return matrix.m_Elements[row * (matrix.width() + 1) + idx];
		}
	};

	struct ConstRowRef
	{
		const AugmentedMatrix& matrix;
		size_t row;
		
		int operator[](size_t idx) const
		{
			return matrix.m_Elements[row * (matrix.width() + 1) + idx];
		}
	};

	void swap(const RowRef& a, const RowRef& b);

	RowRef operator[](size_t idx);
	ConstRowRef operator[](size_t idx) const;

	uint32_t width() const;
	uint32_t height() const;
private:
	std::vector<int> m_Elements;
	uint32_t m_Width;
	uint32_t m_Height;
};

uint32_t AugmentedMatrix::width() const
{
	return m_Width;
}

uint32_t AugmentedMatrix::height() const
{
	return m_Height;
}
