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

#ifndef CELLULARAUTOMATON_HPP
#define CELLULARAUTOMATON_HPP

#include <QColor>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QDebug>

#include <fstream>
#include <string>

#include <Exception.hpp>


namespace Scripting
{
class Calculator;
class FunctionOffset;
class FunctionStat;

/**
 * @brief The CellularAutomaton class represents cellular automaton.
 *
 * The CellularAutomaton class represents cellular automaton. It contains information about the grid, states available in the automaton and it's rules. The type used for representing cell's state is
 * quint16.
 */
class CellularAutomaton:
        public QObject
{
    Q_OBJECT

private:
    enum class Instruction : char {
        RETURN_STATE,
        COMPARATOR_EQUAL,
        COMPARATOR_DIFFERENT,
        COMPARATOR_GREATER,
        COMPARATOR_GREATEER_EQUAL,
        COMPARATOR_LESS,
        COMPARATOR_LESS_EQUAL
    };


    /**
     * @brief The StateRegisterEntry struct
     * This struct is used to build states register. Each instance contains informations about one state available in the automaton.
     */
    struct StateRegisterEntry
    {
        /** @brief Name for this state. Multiple states can have the same names. */
        QString name;

        /** @brief Color used to paint hist state. */
        QColor color;
    };

    struct ScriptLine {
        Instruction instruction;
        Calculator *leftExpression;
        Calculator *rightExpression;
        unsigned int data1;
        unsigned int data2;
    };

private:
    QString _name;

    /** @brief Automaton's grid width. */
    size_t _width;

    /** @brief Automaton's grid height. */
    size_t _height;

    /** @brief This array stores informations about current states of all cells in automaton. */
    quint16 **_grid;

    /** @brief Stores informations about available cell's states. */
    QVector<StateRegisterEntry> _stateRegister;

    /** @brief Contains true if and only if construction of object is finished. */
    bool _constructed;


    quint16 _defaultState;
    FunctionOffset *_functionOffsetInstance;
    FunctionStat *_functionStatInstance;
    size_t _scriptSize;
    size_t _firstLine;
    ScriptLine *_rule;


    /**
     * @brief _deleteGrid
     * deletes memory alocated for grid. Pointers to grids are set to <code>nullptr</code>
     * This function
     */
    void deleteGrid();

public:
    /**
     * @brief STATES_LIMIT
     * limits number of states that can be used in automaton.
     */
    static const size_t STATES_LIMIT;

    static const size_t SCRIPT_LENGTH_LIMIT;

    static const size_t GRID_SIZE_LIMIT;

    static CellularAutomaton *readFromFile(QString path);

    /**
     * @brief CellularAutomaton
     * creates empty automaton -- grid sieze and all states are must be set later
     */
    CellularAutomaton();

    /**
     * @brief CellularAutomaton
     * creates that contains given number of states in register. Each has default name ("STATE" + state number) and random color. Grid is not
     * alolocated. New states can be added later. Grid is not allocated.
     * @param statesNumber number of states initially put to register
     */
    explicit CellularAutomaton(const quint16 statesNumber);

    /**
     * @brief CellularAutomaton
     * creates that contains given number of states in register. Each has default name ("STATE" + state number) and random color. Grid is
     * not alolocated for given size. States register and grid size can be modified later.
     * @param statesNumber number of states initially put to register
     * @param width number of collumns in grid
     * @param height number of rows in grid
     */
    CellularAutomaton(const quint16 statesNumber, const size_t width, const size_t height);

    /**
     * @brief ~CellularAutomaton
     * destructor of the <code>CellularAutomaton</code> object.
     */
    virtual ~CellularAutomaton();

    /**
     * @brief getStatesNumber
     * returns number of states available in automaton
     * @return number of states in register
     */
    size_t statesNumber();

    /**
     * @brief newState
     * creates new state and puts it at the end of register. It's number equal to last already existing in registry state number increased by 1.
     * @param name name for new state
     * @param color color fon new state
     * @return number of new state
     */
    quint16 newState(const QString name, const QColor color);

    /**
     * @brief setStateColor
     * sets color of given state.
     * @param state state to be modified
     * @param newColor new color for the state
     */
    void setStateColor(quint16 state, QColor newColor);

    /**
     * @brief getStateColor
     * returns color of given state.
     * @param state color of this state will be returned
     * @return color of state identified by number given in argument
     */
    QColor stateColor(quint16 state) const;

    /**
     * @brief setStateColor
     * sets color of given state.
     * @param state state to be modified
     * @param newColor new color for the state
     */
    void setStateName(quint16 state, QString newName);

    /**
     * @brief getStateName
     * returns name of given state.
     * @param state name of this state will be returned
     * @return name of state identified by number given in argument
     */
    QString stateName(quint16 state) const;

    /**
     * @brief gridWidth
     * @return number of columns in grid
     */
    unsigned int gridWidth() const;

    /**
     * @brief gridHeight
     * @return number of rows in grid
     */
    unsigned int gridHeight() const;

    /**
     * @brief resizeGrid
     * resizes the grid in cellular automaton. Deletes previously used memory, and allocates new. All calls are set to 0.
     * @param width new width for grid
     * @param height new height for grid
     */
    void resizeGrid(size_t width, size_t height);

    /**
      * @brief setCellState
      * sets stete of cell at given coordinates in current generation.
      * @throws IllegalArgumentException when given state is bigger than <code>_maxValidState</code>
      * @throws IndexOutOfBoundsException when cell at given row and column is outside grid
      */
    void setCellState(size_t x, size_t y, quint16 state);

    /**
      * @brief Function cellState
      * returns stete of cell at given coordinates in current generation.
      * @throws IndexOutOfBoundsException when cell at given row and column is outside grid
      */
    quint16 cellState(size_t x, size_t y) const;


    void compileScript(std::istream &inputStream, size_t length, size_t firstLine, quint16 defaultState);

    QString name() const;
    void setName(const QString &name);

    void saveToFile(QString path);

signals:
    /**
     * @brief Signal registerChanged
     * This signal is emited when <code>_stateRegister</code> changes. Constructor will not emit this signal.
     */
    void registerChanged(quint16 state);


public slots:
    void nextGeneration();
};

}

#endif // CELLULARAUTOMATON_HPP
