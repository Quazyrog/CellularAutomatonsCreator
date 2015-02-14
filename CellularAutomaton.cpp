/* Copyright 2014, 2015 Wojciech Matusiak
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

#include "CellularAutomaton.hpp"


namespace Scripting
{

const size_t CellularAutomaton::STATES_LIMIT = 65535;


CellularAutomaton::CellularAutomaton()
{
    _constructed = false;
    _height = 0;
    _width = 0;
    _currentGeneration = nullptr;
    _oldGeneration = nullptr;

    _rule = nullptr;

    _constructed = true;
}


CellularAutomaton::CellularAutomaton(const quint16 statesNumber) :
    CellularAutomaton()
{
    _constructed = false;
    try {
        for (quint16 i = 0; i < statesNumber; ++i)
            newState(QString("STATE") + QString::number(i), QColor(qrand() % 256, qrand() % 256, qrand() % 256));
    } catch (Exceptions::StateLimitReachedException e) {
        qWarning() << "States limit reached. Only " << _stateRegister.size() << " states were created.";
    }
    _constructed = true;
}


CellularAutomaton::CellularAutomaton(const quint16 statesNumber, const size_t width, const size_t height) :
    CellularAutomaton(statesNumber)
{
    resizeGrid(width, height);
}


CellularAutomaton::~CellularAutomaton()
{
    deleteGrid();
}


void CellularAutomaton::deleteGrid()
{
    if (_oldGeneration != nullptr) {
        for (size_t i = 0; i < _width; ++i)
            delete [] _oldGeneration[i];
        delete [] _oldGeneration;
    }
    if (_currentGeneration != nullptr) {
        for (size_t i = 0; i < _width; ++i)
            delete [] _currentGeneration[i];
        delete [] _currentGeneration;
    }
}


size_t CellularAutomaton::getStatesNumber()
{
    return _stateRegister.size();
}


quint16 CellularAutomaton::newState(const QString name, const QColor color)
throw (Exceptions::StateLimitReachedException)
{
    if (static_cast<size_t>(_stateRegister.size()) >= STATES_LIMIT)
        throw Exceptions::StateLimitReachedException();

    StateRegisterEntry entry;
    entry.name = name;
    entry.color = color;
    _stateRegister.append(entry);

    if (_constructed)
        emit registerChanged(static_cast<quint16>(_stateRegister.size() - 1));

    return static_cast<quint16>(_stateRegister.size() - 1);
}




void CellularAutomaton::setStateColor(quint16 state, QColor newColor)
throw (Exceptions::IndexOutOfBoundsException)
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    if (_constructed)
        emit(registerChanged(state));

    _stateRegister[state].color = newColor;
}


QColor CellularAutomaton::getStateColor(quint16 state) const
throw (Exceptions::IndexOutOfBoundsException)
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    return _stateRegister[state].color;
}


void CellularAutomaton::setStateName(quint16 state, QString newName)
throw (Exceptions::IndexOutOfBoundsException)
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    if (_constructed)
        emit(registerChanged(state));

    _stateRegister[state].name = newName;
}


QString CellularAutomaton::getStateName(quint16 state) const
throw (Exceptions::IndexOutOfBoundsException)
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    return _stateRegister[state].name;
}


unsigned int CellularAutomaton::getGridWidth() const
{
    return _width;
}


unsigned int CellularAutomaton::getGridHeight() const
{
    return _height;
}


void CellularAutomaton::resizeGrid(size_t width, size_t height)
{
    if (width == 0 || height == 0 || (width == _width && height == _height))
        return;

    deleteGrid();

    _currentGeneration = new quint16* [width];
    _oldGeneration = new quint16* [width];
    for (size_t i = 0; i < width; ++i) {
        _currentGeneration[i] = new quint16 [height];
        memset(_currentGeneration[i], 0, height * sizeof(quint16));
        _oldGeneration[i] = new quint16 [height];
        memset(_currentGeneration[i], 0, height * sizeof(quint16));
    }

    _width = width;
    _height = height;
}


void CellularAutomaton::setCellState(size_t x, size_t y, quint16 state)
throw (Exceptions::IllegalArgumentException, Exceptions::IndexOutOfBoundsException)
{
    if (state >= getStatesNumber())
        throw Exceptions::IllegalArgumentException();
    if (y >= _height || x >= _width)
        throw Exceptions::IndexOutOfBoundsException();

    _currentGeneration[x][y] = state;
}


quint16 CellularAutomaton::getCellState(size_t x, size_t y) const
throw (Exceptions::IndexOutOfBoundsException)
{
    if (y >= _height || x >= _width)
        throw Exceptions::IndexOutOfBoundsException();

    return _currentGeneration[x][y];
}

}
