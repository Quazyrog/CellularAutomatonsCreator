/* Copyright 2014 Wojciech Matusiak
 *
 * This file is part of CellularAutomatonCreator.
 *
 * CellularAutomatonCreator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CellularAutomatonCreator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CellularAutomatonCreator.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CELLULARAUTOMATON_HPP
#define CELLULARAUTOMATON_HPP


#include <cstdio>
#include <exception>
#include <iostream>
#include <iomanip>

#include <QString>
#include <QSet>
#include <QObject>
#include <QList>
#include <QException>
#include <QStringList>
#include <QSet>

#include "CellInfo.hpp"


using std::string;


class CellInfo;


enum MathexprCharType {DIGIT,
                       OPERATOR,
                       BRACKET_OPEN,
                       BRACKET_CLOSE,
                       MINUS,
                       NOTHING};


struct SyntaxCheckResult
{
    QString msg;
    int invalid;

    SyntaxCheckResult() {}
    SyntaxCheckResult(QString m, int p);
};


class ScriptBrick
{
    friend class CellularAutomaton;
    friend class ScriptViweWidget;

public:
    virtual ~ScriptBrick();

    virtual StatusT exec(CellInfo *cell);
};


class ScriptBrickIf :
        public ScriptBrick
{
    friend class CellularAutomaton;
    friend class ScriptViweWidget;

public:
    enum ComparisionOperators { EQUAL,
                                NOT_EQUAL,
                                LESS,
                                LESS_OR_EQUAL,
                                GREATER,
                                GREATER_OR_EQUAL };
    ComparisionOperators comparisionOperator;
    QString leftExpression, rightExpression;
    ScriptBrick *then, *next;

    ScriptBrickIf(QString l, ComparisionOperators c, QString r);
    virtual ~ScriptBrickIf();


    virtual StatusT exec(CellInfo *cell);
};


class ScriptBrickReturn :
        public ScriptBrick
{
    friend class CellularAutomaton;
    friend class ScriptViweWidget;

    StatusT value;

public:
    ScriptBrickReturn(StatusT v);
    virtual ~ScriptBrickReturn();

    virtual StatusT exec(CellInfo *cell);
};



class CellularAutomaton
{
    friend class CellInfo;

private:
    static long long int calculateMathexpr(const char *expr, size_t length);

    static QSet<char> digits, operators;

    size_t gridWidth, gridHeight;
    StatusT **oldGrid, **grid;
    ScriptBrick *startScript;

    CellInfo *collectCellInfo(size_t x, size_t y);
    CellInfo *info;
    void deleteGrid();

public:
    static long long int calculateMathexpr(const char *expr);
    static SyntaxCheckResult checkSyntaxOfMathexpr(const char *expr);
    static void initialize();

    CellularAutomaton();
    ~CellularAutomaton();

    StatusT get(size_t x, size_t y);
    void set(size_t x, size_t y, StatusT stat);

    void setScript(ScriptBrick *start);
    void createGrid(size_t w, size_t h);

public slots:
    void nextGeneration();
    void clear();

};

#endif // CELLULARAUTOMATON_HPP
