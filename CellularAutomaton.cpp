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

#include "Calculator.hpp"
#include "FunctionOffset.hpp"
#include "FunctionStat.hpp"

namespace Scripting
{

const size_t CellularAutomaton::STATES_LIMIT = 65535;
const size_t CellularAutomaton::SCRIPT_LENGTH_LIMIT = 1024;
const size_t CellularAutomaton::GRID_SIZE_LIMIT = 60000;


CellularAutomaton::CellularAutomaton()
{
    _constructed = false;

    _name = "New Automaton";

    _height = 0;
    _width = 0;
    _currentGeneration = nullptr;
    _oldGeneration = nullptr;

    _rule = nullptr;
    _functionOffsetInstance = new FunctionOffset(this);
    _functionStatInstance = new FunctionStat(this);

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
    delete [] _rule;
    delete _functionOffsetInstance;
    delete _functionStatInstance;
}



QString CellularAutomaton::name() const
{
    return _name;
}

void CellularAutomaton::setName(const QString &name)
{
    _name = name;
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


size_t CellularAutomaton::statesNumber()
{
    return _stateRegister.size();
}


quint16 CellularAutomaton::newState(const QString name, const QColor color)
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
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    if (_constructed)
        emit(registerChanged(state));

    _stateRegister[state].color = newColor;
}


QColor CellularAutomaton::stateColor(quint16 state) const
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    return _stateRegister[state].color;
}


void CellularAutomaton::setStateName(quint16 state, QString newName)
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    if (_constructed)
        emit(registerChanged(state));

    _stateRegister[state].name = newName;
}


QString CellularAutomaton::stateName(quint16 state) const
{
    if (state >= static_cast<quint16>(_stateRegister.size()))
        throw Exceptions::IndexOutOfBoundsException();

    return _stateRegister[state].name;
}


unsigned int CellularAutomaton::gridWidth() const
{
    return _width;
}


unsigned int CellularAutomaton::gridHeight() const
{
    return _height;
}


void CellularAutomaton::resizeGrid(size_t width, size_t height)
{
    if (width == 0 || height == 0)
        throw Exceptions::IllegalArgumentException();
    if (width == _width && height == _height)
        return;

    if (width * height > GRID_SIZE_LIMIT)
        throw Exceptions::IndexOutOfBoundsException();

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
{
    if (state >= statesNumber())
        throw Exceptions::IllegalArgumentException();
    if (y >= _height || x >= _width)
        throw Exceptions::IndexOutOfBoundsException();

    _currentGeneration[x][y] = state;
}


quint16 CellularAutomaton::cellState(size_t x, size_t y) const
{
    if (y >= _height || x >= _width)
        throw Exceptions::IndexOutOfBoundsException();

    return _currentGeneration[x][y];
}


CellularAutomaton *CellularAutomaton::readFromFile(QString path)
{
    qDebug() << "Reading file" << path;
    CellularAutomaton *result = new CellularAutomaton;

    std::ifstream inputStream(path.toStdString());
    if (!inputStream.is_open() || !inputStream.good()) {
        qWarning() << "Failed to open " << path;
        throw Exceptions::IOException();
    }

    std::string name;
    getline(inputStream, name);
    result->setName(QString::fromStdString(name));
    qDebug() << "Name is" << QString::fromStdString(name);

    size_t statesNumber;
    inputStream >> statesNumber;
    qDebug() << "Reading" << statesNumber << "states";
    if (statesNumber > STATES_LIMIT)
        throw Exceptions::SyntaxErrorException(-1, "States number bigger than limit");
    for (quint16 i = 0; i < statesNumber; ++i) {
        std::string name;
        int r, g, b;
        inputStream >> name >> r >> g >> b;
        result->newState(QString::fromStdString(name), QColor(r, g, b));
    }

    size_t scriptLength;
    inputStream >> scriptLength;
    qDebug() << "Reading script of size" << scriptLength;
    result->compileScript(inputStream, scriptLength);


    unsigned int width, height;
    inputStream >> width >> height;
    qDebug() << "Reading grid of size" << width << "x" << height;
    if (width * height == 0)
        throw Exceptions::SyntaxErrorException(-1, "Grid size is 0");
    if (width * height > GRID_SIZE_LIMIT)
        throw Exceptions::SyntaxErrorException(-1, "Grid too large");
    result->resizeGrid(width, height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            quint16 state;
            inputStream >> state;
            try {
                result->setCellState(x, y, state);
            } catch (Exceptions::IllegalArgumentException &except) {
                throw Exceptions::SyntaxErrorException(-1, "Invalid state");
            }
        }
    }

    qDebug() << "File read successfully";
    return result;
}

void CellularAutomaton::compileScript(std::istream &inputStream, size_t length)
{
    if (length > SCRIPT_LENGTH_LIMIT)
        throw Exceptions::SyntaxErrorException(-1, "Script too long");

    ScriptLine *compilationBuffer = new ScriptLine [length];
    bool *definedLines = new bool [length];
    memset(definedLines, 0, sizeof(bool) * length);
    auto deleteArrays = [&]() {
        delete [] compilationBuffer;
        delete [] definedLines;
    };

    for (quint16 i = 0; i < length; ++i) {
        size_t lineNumber;
        inputStream >> lineNumber;
        if (length <= lineNumber) {
            deleteArrays();
            throw Exceptions::SyntaxErrorException(-1, "Invalid line number");
        }
        if (definedLines[lineNumber]) {
            deleteArrays();
            throw Exceptions::SyntaxErrorException(lineNumber, "Line redefined");
        }
        definedLines[lineNumber] = true;

        std::string token;
        inputStream >> token;
        ScriptLine &line = compilationBuffer[lineNumber];
        if (token == "IF") {
            std::string left, sign, right;
            inputStream >> left >> sign >> right;
            if (sign == "=")
                line.instruction = Instruction::COMPARATOR_EQUAL;
            if (sign == "<>")
                line.instruction = Instruction::COMPARATOR_DIFFERENT;
            if (sign == "<")
                line.instruction = Instruction::COMPARATOR_GREATER;
            if (sign == "<=")
                line.instruction = Instruction::COMPARATOR_GREATEER_EQUAL;
            if (sign == ">")
                line.instruction = Instruction::COMPARATOR_LESS;
            if (sign == ">=")
                line.instruction = Instruction::COMPARATOR_LESS_EQUAL;

            try {
                line.leftExpression = new Calculator();
                line.leftExpression->installFunction3D(_functionOffsetInstance);
                line.leftExpression->installFunction2D(_functionStatInstance);
                line.leftExpression->parseExpression(QString::fromStdString(left));

                line.rightExpression = new Calculator();
                line.rightExpression->installFunction3D(_functionOffsetInstance);
                line.rightExpression->installFunction2D(_functionStatInstance);
                line.rightExpression->parseExpression(QString::fromStdString(right));
            } catch (Exceptions::SyntaxErrorException &err) {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, err.message());
            }

            size_t thenGoto;
            inputStream >> token;
            if (token != "THEN") {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, QString::fromStdString("Expected 'THEN' got '" + token + "'"));
            }
            inputStream >> thenGoto;
            if (thenGoto >= length) {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, "Invalid line number");
            }
            line.data1 = thenGoto;


            size_t elseGoto;
            inputStream >> token;
            if (token != "ELSE") {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, QString::fromStdString("Expected 'ELSE' got '" + token + "'"));
            }
            inputStream >> elseGoto;
            if (elseGoto >= length) {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, "Invalid line number");
            }
            line.data2 = elseGoto;
        } else if (token == "STATE") {
            line.instruction = Instruction::RETURN_STATE;
            line.leftExpression = nullptr;
            line.rightExpression = nullptr;

            quint16 state;
            inputStream >> state;
            if (state >= statesNumber()) {
                deleteArrays();
                throw Exceptions::SyntaxErrorException(lineNumber, "No such state");
            }
            line.data1 = state;
        } else {
            deleteArrays();
            throw Exceptions::SyntaxErrorException(lineNumber, QString::fromStdString("Unknown token '" + token + "'"));
        }
    }

    delete [] definedLines;
    delete [] _rule;
    _rule = compilationBuffer;
    _scriptSize = length;
}

}
