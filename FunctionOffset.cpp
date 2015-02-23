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

#include "FunctionOffset.hpp"

#include"CellularAutomaton.hpp"




namespace Scripting {

FunctionOffset::FunctionOffset(CellularAutomaton *automaton)
{
    if (automaton == nullptr)
        throw Exceptions::NullPointerException();

    _automaton = automaton;
    _x = 0;
    _y = 0;
}


FunctionOffset::~FunctionOffset()
{

}


void FunctionOffset::setXY(unsigned int x, unsigned int y)
{
    _x = x;
    _y = y;
}


QString FunctionOffset::name() const
{
    return "OFFSET";
}


bool FunctionOffset::isVolatile() const
{
    return true;
}


double FunctionOffset::valueOf(double arg1, double arg2)
{
    return _automaton->cellState(_x + arg1, _y + arg2);
}


} // namespace Scripting

