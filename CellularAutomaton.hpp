#ifndef CELLULARAUTOMATON_HPP
#define CELLULARAUTOMATON_HPP


#include <cstdio>
#include <exception>
#include <iostream>
#include <unordered_set>

#include <QString>
#include <QSet>
#include <QObject>
#include <QList>
#include <QException>
#include <QStringList>

#include "CellInfo.hpp"


using std::string;
using std::unordered_set;


class CellInfo;


class CellularAutomaton
{
    friend class CellInfo;

    class ScriptBrick
    {
        friend class CellularAutomaton;

    public:
        virtual ~ScriptBrick();

        virtual StatusT exec(CellInfo *cell);
    };

    static QSet<char> digits, operators;
    static long long int calculateMathexpr(const char *expr, size_t length);

public:
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

        SyntaxCheckResult(QString m, int p);
    };


    class ScriptBrickIf
    {
        friend class CellularAutomaton;

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

    class ScriptBrickReturn : ScriptBrick
    {
        friend class CellularAutomaton;

        StatusT value;

    public:
        ScriptBrickReturn(StatusT v);
        virtual ~ScriptBrickReturn();

        virtual StatusT exec(CellInfo *cell);
    };

public:
    static long long int calculateMathexpr(const char *expr);
    static SyntaxCheckResult checkSyntaxOfMathexpr(const char *expr);
    static void initialize();

    CellularAutomaton();
};

#endif // CELLULARAUTOMATON_HPP
