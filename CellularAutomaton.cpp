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
    _defaultState = 0;
    _firstLine = 0;
    _grid = nullptr;

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
    if (_grid != nullptr) {
        for (size_t i = 0; i < _width; ++i)
            delete [] _grid[i];
        delete [] _grid;
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

    _grid = new quint16* [width];
    for (size_t i = 0; i < width; ++i) {
        _grid[i] = new quint16 [height];
        memset(_grid[i], 0, height * sizeof(quint16));
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

    _grid[x][y] = state;
}


quint16 CellularAutomaton::cellState(size_t x, size_t y) const
{
    if (y >= _height || x >= _width)
        throw Exceptions::IndexOutOfBoundsException();

    return _grid[x][y];
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

    size_t scriptLength, firstLineNumber, defaultState;
    inputStream >> scriptLength >> firstLineNumber >> defaultState;
    qDebug() << "Reading script of size" << scriptLength;
    result->compileScript(inputStream, scriptLength, firstLineNumber, defaultState);


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

void CellularAutomaton::compileScript(std::istream &inputStream, size_t length, size_t firstLine, quint16 defaultState)
{
    if (length > SCRIPT_LENGTH_LIMIT)
        throw Exceptions::SyntaxErrorException(-1, "Script too long");
    if (firstLine >= length)
        throw Exceptions::SyntaxErrorException(-1, "Invalid first line number");
    if (defaultState >= _stateRegister.size())
        throw Exceptions::SyntaxErrorException(-1, "Invalid default state");
    _firstLine = firstLine;
    _defaultState = defaultState;

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
            if (sign == ">")
                line.instruction = Instruction::COMPARATOR_GREATER;
            if (sign == ">=")
                line.instruction = Instruction::COMPARATOR_GREATEER_EQUAL;
            if (sign == "<")
                line.instruction = Instruction::COMPARATOR_LESS;
            if (sign == "<=")
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


void CellularAutomaton::saveToFile(QString path)
{
    using std::endl;

    qDebug() << "Saving automaton" << _name << "as" << path;
    std::ofstream outStream(path.toStdString());
    if (!outStream.is_open() || !outStream.good())
        throw Exceptions::IOException();
    outStream << _name.toStdString() << endl;

    outStream << _stateRegister.size() << endl;
    for (StateRegisterEntry state : _stateRegister)
        outStream << state.name.toStdString() << " " << state.color.red() << " " << state.color.green() << " " << state.color.blue() << std::endl;

    outStream << _scriptSize << " " << _firstLine << " " << _defaultState << endl;
    for (unsigned int i = 0; i < _scriptSize; ++i) {
        ScriptLine &line = _rule[i];
        if (line.instruction == Instruction::RETURN_STATE) {
            outStream << i << " STATE " << line.data1 << endl;
        } else {
            outStream << i << " IF " << line.leftExpression->toString().toStdString();
            switch (line.instruction) {
            case Instruction::COMPARATOR_EQUAL:
                outStream << " = ";
                break;
            case Instruction::COMPARATOR_DIFFERENT:
                outStream << " <> ";
                break;
            case Instruction::COMPARATOR_LESS:
                outStream << " < ";
                break;
            case Instruction::COMPARATOR_LESS_EQUAL:
                outStream << " <= ";
                break;
            case Instruction::COMPARATOR_GREATER:
                outStream << " > ";
                break;
            case Instruction::COMPARATOR_GREATEER_EQUAL:
                outStream << " >= ";
                break;
            default:
                qWarning() << "Unexpected switch value!";
                break;
            }
            outStream << line.rightExpression->toString().toStdString() << " THEN " << line.data1 << " ELSE " << line.data2 << endl;
        }
    }

    outStream << _width << " " << _height << endl;
    for (unsigned int y = 0; y < _height; ++y) {
        for (unsigned int x = 0; x < _width; ++x)
            outStream << _grid[x][y] << " ";
        outStream << endl;
    }

    outStream.close();
}


void CellularAutomaton::nextGeneration()
{
    static quint16 **newGrid = nullptr;
    static size_t tmpWidth = 0, tmpHeight = 0;
    constexpr unsigned int depthLimit = 64;

    if (tmpWidth != _width || tmpHeight != _height) {
        if (newGrid != nullptr) {
            for (unsigned int i = 0; i < tmpWidth; ++i)
                delete [] newGrid[i];
            delete [] newGrid;
        }

        newGrid = new quint16* [_width];
        for (unsigned int i = 0; i < _width; ++i)
            newGrid[i] = new quint16 [_height];
        tmpWidth = _width;
        tmpHeight = _height;
    }

    for (unsigned int x = 0; x < _width; ++x) {
        for (unsigned int y = 0; y < _height; ++y) {
            _functionOffsetInstance->setXY(x, y);
            _functionStatInstance->setXY(x, y);

            quint16 state = _defaultState;
            ScriptLine *line = _rule + _firstLine;
            for (unsigned int depth = 1; depth <= depthLimit; ++depth) {
                if (line->instruction == Instruction::RETURN_STATE) {
                    state = line->data1;
                    break;
                }

                double leftValue;
                double rightValue;

                bool comparisionResult;
                try {
                    leftValue = line->leftExpression->value();
                    rightValue = line->rightExpression->value();

                    switch (line->instruction) {
                    case Instruction::COMPARATOR_EQUAL:
                        comparisionResult = (leftValue == rightValue);
                        break;
                    case Instruction::COMPARATOR_DIFFERENT:
                        comparisionResult = (leftValue != rightValue);
                        break;
                    case Instruction::COMPARATOR_LESS:
                        comparisionResult = (leftValue < rightValue);
                        break;
                    case Instruction::COMPARATOR_LESS_EQUAL:
                        comparisionResult = (leftValue <= rightValue);
                        break;
                    case Instruction::COMPARATOR_GREATER:
                        comparisionResult = (leftValue > rightValue);
                        break;
                    case Instruction::COMPARATOR_GREATEER_EQUAL:
                        comparisionResult = (leftValue >= rightValue);
                        break;
                    default:
                        qWarning() << "Unexpected switch value!";
                        break;
                    }
                } catch (Exceptions::IndexOutOfBoundsException &except) {
                    comparisionResult = false;
                }

                if (comparisionResult)
                    line = _rule + line->data1;
                else
                    line = _rule + line->data2;
            }
            newGrid[x][y] = state;
        }
    }

    quint16 **tmp = _grid;
    _grid = newGrid;
    newGrid = tmp;
}


void CellularAutomaton::fillGrid(quint16 state)
{
    if (state >= _stateRegister.size())
        throw Exceptions::IllegalArgumentException();
    for (unsigned int x = 0; x < _width; ++x) {
        for (unsigned int y = 0; y < _height; ++y)
            _grid[x][y] = state;
    }
}

} //Namespace Scripting
