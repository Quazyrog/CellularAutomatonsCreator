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

public:
    class SyntaxExrrorException : QException
    {
        QString message;
        int position;
        SyntaxExrrorException(const SyntaxExrrorException *other);
    public:
        SyntaxExrrorException(QString msg, int pos);

        void raise() const;
        SyntaxExrrorException *clone() const;

        QString getMessage();
        int getPosition();
    };

private:
    enum MathexprCharType {DIGIT,
                           OPERATOR,
                           BRACKET_OPEN,
                           BRACKET_CLOSE,
                           MINUS,
                           NOTHING};
    static QSet<char> digits, operators;
    static long long int calculateMathexpr(const char *expr, size_t length);

    class ScriptBrick
    {
        friend class CellularAutomaton;

    public:
        virtual ~ScriptBrick();

        virtual StatusT exec(CellInfo *cell);
    };

    class ScriptBrickIf
    {
        friend class CellularAutomaton;

        enum ComparisionOperators { NONE,
                                    EQUAL,
                                    NOT_EQUAL,
                                    LESS,
                                    LESS_OR_EQUAL,
                                    GREATER,
                                    GREATER_OR_EQUAL };
        ComparisionOperators comparisionOperator;
        QString leftExpression, rightExpression;
        ScriptBrick *then, *next;

    public:
        ScriptBrickIf(QString condition) throw (SyntaxExrrorException *);
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
    static int checkSyntaxOfMathexpr(const char *expr) throw (SyntaxExrrorException *);
    static void initialize();

    CellularAutomaton();
};

#endif // CELLULARAUTOMATON_HPP
