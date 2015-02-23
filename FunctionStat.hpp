/* Copyright 2015 Wojciech Matusiak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTING_FUNCTIONSTAT_HPP
#define SCRIPTING_FUNCTIONSTAT_HPP


#include <Calculator.hpp>



namespace Scripting {

class CellularAutomaton;


class FunctionStat : public CalculatorFunction2D
{
    CellularAutomaton *_automaton;
    unsigned int _x;
    unsigned int _y;

    unsigned char *_cache;
    unsigned int _cacheSize;
    bool _cacheExpired;

public:
    FunctionStat(CellularAutomaton *automaton);
    virtual ~FunctionStat();

    void setXY(unsigned int x, unsigned int y);
    void updateCache();

    virtual QString name() const override;
    virtual double valueOf(double arg) override;
    virtual bool isVolatile() const override;
};

} // namespace Scripting

#endif // SCRIPTING_FUNCTIONSTAT_HPP
