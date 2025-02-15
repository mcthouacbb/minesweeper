#include "augmented_matrix.h"
#include <algorithm>

void AugmentedMatrix::swap(const RowRef& a, const RowRef& b)
{
	std::swap_ranges(&a[0], &a[width()], &b[0]);
}

AugmentedMatrix::RowRef AugmentedMatrix::operator[](size_t idx)
{
	return RowRef{*this, idx};
}

AugmentedMatrix::ConstRowRef AugmentedMatrix::operator[](size_t idx) const
{
	return ConstRowRef{*this, idx};
}