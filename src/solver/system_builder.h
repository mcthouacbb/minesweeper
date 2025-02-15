#pragma once

#include "../types.h"
#include <vector>
#include <unordered_map>

struct Equation
{
    std::vector<Point> lhs;
    int rhs;

    Equation& addVariable(Point pt);
    void setRHS(int value);
};

inline Equation& Equation::addVariable(Point pt)
{
    lhs.push_back(pt);
    return *this;
}

inline void Equation::setRHS(int value)
{
    rhs = value;
}

// very specific to minesweeper
class LinearSystemBuilder
{
public:
    LinearSystemBuilder() = default;

    Equation& addEquation();
    void build();
private:
    void addVariable(Point pt);
    std::vector<Equation> m_Equations;
    std::unordered_map<Point, uint32_t, PointHash> cellToVar;
    std::vector<Point> varToCell;
};

inline void LinearSystemBuilder::addVariable(Point pt)
{

}

void LinearSystemBuilder::build()
{
    for (const auto& equation : m_Equations)
    {
        for (auto var : equation.lhs)
        {
            addVariable(var);
        }
    }

    
}
