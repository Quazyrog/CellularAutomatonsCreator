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
    static QSet<char> digits, operators;
    static long long int calculateMathexpr(const char *expr, size_t length);

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
