#pragma once

#include "../types.h"
#include "augmented_matrix.h"
#include <vector>

struct Term
{
    int coeff;
    uint32_t varID;
};

struct Equation
{
    std::vector<Term> lhs;
    int rhs;

    void addTerm(int coeff, uint32_t varID);
    void setRHS(int value);
};

inline void Equation::addTerm(int coeff, uint32_t varID)
{
    lhs.push_back({coeff, varID});
}

inline void Equation::setRHS(int value)
{
    rhs = value;
}

class LinearSystem
{
public:
    LinearSystem() = default;
    LinearSystem(const AugmentedMatrix& matrix);

    Equation& addEquation();

    AugmentedMatrix toAugmentedMatrix(uint32_t numVariables) const;

    const std::vector<Equation>& equations() const;
private:
    std::vector<Equation> m_Equations;
};

inline LinearSystem::LinearSystem(const AugmentedMatrix& matrix)
{
    m_Equations.reserve(matrix.height());
    for (int i = 0; i < matrix.height(); i++)
    {
        Equation eq = {};
        for (int j = 0; j < matrix.width(); j++)
            if (matrix[i][j] != 0)
                eq.addTerm(matrix[i][j], j);
        if (eq.lhs.empty())
            continue;
        eq.setRHS(matrix[i][matrix.width()]);
        m_Equations.push_back(eq);
    }
}

inline Equation& LinearSystem::addEquation()
{
    m_Equations.push_back({});
    return m_Equations.back();
}

inline AugmentedMatrix LinearSystem::toAugmentedMatrix(uint32_t numVariables) const
{
    AugmentedMatrix result{numVariables, static_cast<uint32_t>(m_Equations.size())};
    uint32_t rowIdx = 0;
    for (auto& equation : m_Equations)
    {
        for (auto& term : equation.lhs)
        {
            result[rowIdx][term.varID] = term.coeff;
        }
        result[rowIdx][result.width()] = equation.rhs;
        rowIdx++;
    }
    return result;
}

const std::vector<Equation>& LinearSystem::equations() const
{
    return m_Equations;
}
