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

#include "FunctionStat.hpp"

#include "CellularAutomaton.hpp"

namespace Scripting {

FunctionStat::FunctionStat(CellularAutomaton *automaton)
{
    if (_automaton == nullptr)
        throw Exceptions::NullPointerException();

    _automaton = automaton;
    _cacheExpired = true;
    _cache = nullptr;
    _cacheSize = 0;
    _x = 0;
    _y = 0;
}


FunctionStat::~FunctionStat()
{
    delete [] _cache;
}


void FunctionStat::setXY(unsigned int x, unsigned int y)
{
    _cacheExpired = false;
    _x = x;
    _y = y;
}


void FunctionStat::updateCache()
{
    if (_cacheSize != _automaton->statesNumber()) {
        delete [] _cache;
        _cache = new unsigned char [_automaton->statesNumber()];
    }

    int width = _automaton->gridWidth();
    int height = _automaton->gridHeight();
    memset(_cache, 0, sizeof(unsigned char) * _cacheSize);
    for (int xv = -1; xv <= 1; xv++) {
        for (int yv = -1; yv <= 1; yv++) {
            int x = _x + xv;
            int y = _y + yv;
            if (x < 0 || y < 0 || x >= static_cast<int>(width) || y >= static_cast<int>(height))
                continue;
            ++_cache[_automaton->cellState(_x + xv, _y + yv)];
        }
    }
}


QString FunctionStat::name() const
{
    return "STAT";
}


bool FunctionStat::isVolatile() const
{
    return true;
}


double FunctionStat::valueOf(double arg1)
{
    if (_cacheExpired || _cacheSize != _automaton->statesNumber())
        updateCache();

    return _cache[static_cast<unsigned int>(arg1)];
}

} // namespace Scripting

