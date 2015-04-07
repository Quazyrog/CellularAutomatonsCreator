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
 * The CellularAutomaton class represents cellular automaton. It contains information about the grid, states available in the automaton and it's rules. The type used for representing cell's state is
 * quint16.
 */
class CellularAutomaton:
        public QObject
{
    Q_OBJECT

private:
    /**
     * Instructions used to execute next generation function. They determine how should executed <code>ScriptLine</code> be treated. When automaton reads script, it translates
     * all lines of script into <code>ScriptLine</code> structures, and this enum represents all general cases of script lines.
     */
    enum class Instruction : char {
        RETURN_STATE, ///< Equivalent of <code>STATE ...</state>
        COMPARATOR_EQUAL, ///< Equivalent of <code>IF ... = ... THEN ... ELSE ...</state>
        COMPARATOR_DIFFERENT, ///< Equivalent of <code>IF ... <> ... THEN ... ELSE ...</state>
        COMPARATOR_GREATER, ///< Equivalent of <code>IF ... > ... THEN ... ELSE ...</state>
        COMPARATOR_GREATEER_EQUAL, ///< Equivalent of <code>IF ... >= ... THEN ... ELSE ...</state>
        COMPARATOR_LESS, ///< Equivalent of <code>IF ... < ... THEN ... ELSE ...</state>
        COMPARATOR_LESS_EQUAL ///< Equivalent of <code>IF ... <= ... THEN ... ELSE ...</state>
    };


    /**
     * This struct is used to build states register. Each instance contains informations about one state available in the automaton.
     */
    struct StateRegisterEntry
    {
        /** Name for this state. Multiple states can have the same names. */
        QString name;

        /** Color used to paint hist state. */
        QColor color;
    };


    /**
     * Each instance of this srtucture stores informations about one line from automaton's next generation generating script.
     */
    struct ScriptLine {
        /**
         * Information about witch pattern was used to generate this line
         * @see Instructions
         */
        Instruction instruction;

        /**
         * For conditional statment holds pointer to <code>Calculator</code> with left expression. Unused for <code>STATE</code>.
         */
        Calculator *leftExpression;

        /**
         * For conditional statment holds pointer to <code>Calculator</code> with right expression. Unused for <code>STATE</code>.
         */
        Calculator *rightExpression;

        /**
         * Number after <code>THEN</code> token in conditional statments and number of state of cell in next generation in <code>STATE</code> statments.
         */
        unsigned int data1;

        /**
         * Number after <code>ELSE</code> token in conditional statments. Unused in <code>STATE</code> statments.
         */
        unsigned int data2;
    };

private:
    QString _name;

    /** Automaton's grid width. */
    size_t _width;

    /** Automaton's grid height. */
    size_t _height;

    /** This array stores informations about current states of all cells in automaton. */
    quint16 **_grid;

    /** Stores informations about available cell's states. */
    QVector<StateRegisterEntry> _stateRegister;

    /** Contains true if and only if construction of object is finished. */
    bool _constructed;


    quint16 _defaultState;
    FunctionOffset *_functionOffsetInstance;
    FunctionStat *_functionStatInstance;
    size_t _scriptSize;
    size_t _firstLine;
    ScriptLine *_rule;


    /**
     * deletes memory alocated for grid. Pointers to grids are set to <code>nullptr</code>
     * This function
     */
    void deleteGrid();

public:
    /**
     * limits number of states that can be used in automaton.
     */
    static const size_t STATES_LIMIT;

    /**
     * Maximal number of lines used to define pass function.
     */
    static const size_t SCRIPT_LENGTH_LIMIT;

    /**
     * Maximal number of cells in grid.
     */
    static const size_t GRID_SIZE_LIMIT;

    /**
     * creates empty automaton -- grid sieze and all states are must be set later
     */
    CellularAutomaton();

    /**
     * creates that contains given number of states in register. Each has default name ("STATE" + state number) and random color. Grid is not
     * alolocated. New states can be added later. Grid is not allocated.
     * @param statesNumber number of states initially put to register
     */
    explicit CellularAutomaton(const quint16 statesNumber);

    /**
     * creates that contains given number of states in register. Each has default name ("STATE" + state number) and random color. Grid is
     * not alolocated for given size. States register and grid size can be modified later.
     * @param statesNumber number of states initially put to register
     * @param width number of collumns in grid
     * @param height number of rows in grid
     */
    CellularAutomaton(const quint16 statesNumber, const size_t width, const size_t height);

    /**
     * destructor of the <code>CellularAutomaton</code> object.
     */
    virtual ~CellularAutomaton();

    /**
     * returns number of states available in automaton
     * @return number of states in register
     */
    size_t statesNumber();

    /**
     * creates new state and puts it at the end of register. It's number equal to last already existing in registry state number increased by 1.
     * @param name name for new state
     * @param color color fon new state
     * @return number of new state
     */
    quint16 newState(const QString name, const QColor color);

    /**
     * sets color of given state.
     * @param state state to be modified
     * @param newColor new color for the state
     */
    void setStateColor(quint16 state, QColor newColor);

    /**
     * returns color of given state.
     * @param state color of this state will be returned
     * @return color of state identified by number given in argument
     */
    QColor stateColor(quint16 state) const;

    /**
     * sets color of given state.
     * @param state state to be modified
     * @param newName new color for the state
     */
    void setStateName(quint16 state, QString newName);

    /**
     * returns name of given state.
     * @param state name of this state will be returned
     * @return name of state identified by number given in argument
     */
    QString stateName(quint16 state) const;

    /**
     * Reads script from given stream. Basic informations about script (length, first line and default state) have to be supplied as arguments.
     * Given stream is expected to contain only following lines:
     *  - <code> $NUMBER$ STATE $NUMBER$ </code>
     *  - <code> $NUMBER$ IF $FORMULA$ $COMPARISION_SIGN$ $FORMULA$ THEN $NUMBER$ ELSE $NUMBER$ </code>
     * @param inputStream
     * @param length
     * @param firstLine
     * @param defaultState
     */
    void compileScript(std::istream &inputStream, size_t length, size_t firstLine, quint16 defaultState);

    QString name() const;
    void setName(const QString &name);
    static CellularAutomaton *readFromFile(QString path);
    void saveToFile(QString path);


    /**
     * @brief fillGrid
     * @param state
     */
    void fillGrid(quint16 state = 0);

    /**
      * sets stete of cell at given coordinates in current generation.
      * @throws IllegalArgumentException when given state is bigger than <code>_maxValidState</code>
      * @throws IndexOutOfBoundsException when cell at given row and column is outside grid
      */
    void setCellState(size_t x, size_t y, quint16 state);

    /**
      * returns stete of cell at given coordinates in current generation.
      * @throws IndexOutOfBoundsException when cell at given row and column is outside grid
      */
    quint16 cellState(size_t x, size_t y) const;

    /**
     * @return number of columns in grid
     */
    unsigned int gridWidth() const;

    /**
     * @return number of rows in grid
     */
    unsigned int gridHeight() const;

    /**
     * resizes the grid in cellular automaton. Deletes previously used memory, and allocates new. All calls are set to 0.
     * @param width new width for grid
     * @param height new height for grid
     */
    void resizeGrid(size_t width, size_t height);

signals:
    /**
     * This signal is emited when <code>_stateRegister</code> changes. Constructor will not emit this signal.
     */
    void registerChanged(quint16 state);


public slots:
    /**
     * Cells on gried die and are replaced by new ones following rules defined by pass function.
     */
    void nextGeneration();
};

}

#endif // CELLULARAUTOMATON_HPP
