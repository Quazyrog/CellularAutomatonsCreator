#ifndef CELLULARAUTOMATON_HPP
#define CELLULARAUTOMATON_HPP


#include <cstdio>
#include <exception>
#include <iostream>
#include <unordered_set>

#include <QString>
#include <QSet>
#include <QObject>

using std::string;
using std::unordered_set;



class CellularAutomaton
{
    friend class CellInfo;

    enum MathexprCharType {DIGIT, OPERATOR, BRACKET_OPEN, BRACKET_CLOSE, MINUS, NOTHING};

    static QSet<char> digits, operators;

    static long long int calculateMathexpr(const char *expr, size_t length);

public:
    static long long int calculateMathexpr(const char *expr);
    static int checkSyntaxOfMathexpr(const char *expr);
    static void initialize();

    CellularAutomaton();
};

#endif // CELLULARAUTOMATON_HPP
