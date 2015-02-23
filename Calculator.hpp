/* Copyright 2015 Wojciech Matusiak
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

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <QDebug>

#include <stack>
#include <map>
#include <iostream>

#include "Exception.hpp"

namespace Scripting
{

/**
 * @brief The <code>CalculatorFunction2D</code> class is basic class for calculator's functions with one parameter.
 * Pointer to <code>CalculatorFunction2D</code> may be instelled in calculator. Then it can be interpreted in expressions using syntax <code>name(expr)</code>, where <code>name</code> is function's name and
 * <expr>expr</code> is an expression witch value is given to function <code>CalculatorFunction2D::valueOf</code>. For example this class represents function called <code>foo</code>, witch for given number
 * <code>x</code> returns number <code>2*x+1</code>. Exemplary expression that uses it would be <code>foo(3+foo(0))+1</code>.
 *
 * To create another function you need to create a subclass of this one and reimplement virtual functions.
 *
 * @see CalculatorFunction3D
 * @see Calculator::installFunction2D()
 */
class CalculatorFunction2D
{
public:
    /**
     * @brief The <code>name</code> function resurns name that is used to call it in calculator's expressions.
     * Calculator uses value returned by this function to recongize it calls in expression it parses. When doing it, the calculator is case sensitive. It must consist of letters (<code>isalpha</code>
     * function mustr return <code>true</code> for all letters in name).
     * @return function's name used to recongize it calls (case sensitive). In this case name is <code>"foo"</code>.
     */
    virtual QString name() const;

    /**
     * @brief The <code>valueOf<code> is call to this function for argument <code>arg</code>.
     * During computing value of expression Calculator will use this function to ask, what will be its for given argument. Here <code>valueOf</code> returns <code>2*arg+1</code>, so calculator
     * with expression for example <code>foo(3)+1</code> will return <code>(2*3+1)+1=8</code>.
     * @param arg argument for function
     * @return function's value for given argument. In this case <code>2*arg+1</code>
     */
    virtual double valueOf(double arg);

    /**
     * @brief The <code>isVolatile</code> is used to determine if function's value returned from constant expression is constant.
     * For example this class' function <code>valueOf(arg)</code> retruns <code>2*arg+1</code>. Consequently <code>foo(3)</code> always will be equal <code>7</code> – it is not volatile. In
     * contrary function <code>Scripting::FunctionOffset</code>'s result depends on cell position selected bay automaton. Therefore <code>OFFSET(-1;-1)</code> (state of up-left neighbour of
     * selected cell) will return differtnt values even if it has constant arguments – it is volatile. This will not let Calculator treat it as constant value during optimization.
     * @return <code>true</code> if and only if function is volatile. In such case returns <code>false</code>
     * @see Calculator::optimize
     */
    virtual bool isVolatile() const;
};



/**
 * @brief The <code>CalculatorFunction3D</code> class is basic class for calculator's functions with twoe parameters.
 * Pointer to <code>CalculatorFunction3D</code> may be instelled in calculator. Then it can be interpreted in expressions using syntax <code>name(expr; expr)</code>, where <code>name</code> is
 * function's name and <expr>expr</code> are expressions witch value is given to function <code>CalculatorFunction3D::valueOf</code>. For example this class represents function called
 * <code>fooo</code>, witch for given numbers <code>x</code> and <code>y</code> returns number <code>x*y</code>. Exemplary expression that uses it would be <code>fooo(1+2;fooo(1;1))</code>.
 *
 * To create another function you need to create a subclass of this one and reimplement virtual functions.
 *
 * @see CalculatorFunction2D
 * @see Calculator::installFunction3D()
 */
class CalculatorFunction3D
{
public:
    /**
     * @brief The <code>name</code> function resurns name that is used to call it in calculator's expressions.
     * Calculator uses value returned by this function to recongize it calls in expression it parses. When doing it, the calculator is case sensitive. It must consist of letters (<code>isalpha</code>
     * function mustr return <code>true</code> for all letters in name).
     * @return function's name used to recongize it calls (case sensitive). In this case name is <code>"fooo"</code>.
     */
    virtual QString name() const;

    /**
     * @brief The <code>valueOf<code> is call to this function for arguments <code>arg1</code> and <code>arg2</code>.
     * During computing value of expression Calculator will use this function to ask, what will be its for given argument. Here <code>valueOf</code> returns <code>arg1*arg2</code>, so calculator
     * with expression for example <code>fooo(1+2;fooo(1;1))</code> will return <code>fooo(3;1)=3*1=3</code>.
     * @param arg1 first argument for function
     * @param arg2 second argument for function
     * @return function's value for given arguments. In this case <code>arg1*arg2</code>
     */
    virtual double valueOf(double arg1, double arg2);

    /**
     * @brief The <code>isVolatile</code> is used to determine if function's value returned from constant expression is constant.
     * For example class <code>Scripting::CalculatorFunction2D<code>'s function <code>valueOf(arg)</code> retruns <code>2*arg+1</code>. Consequently <code>foo(3)</code> always will be equal
     * <code>7</code> – it is not volatile. In contrary function <code>Scripting::FunctionOffset</code>'s result depends on cell position selected bay automaton. Therefore <code>OFFSET(-1;-1)</code>
     * (state of up-left neighbour of selected cell) will return differtnt values even if it has constant arguments – it is volatile. This will not let Calculator treat it as constant value
     * during optimization.
     * @return <code>true</code> if and only if function is volatile. In such case returns <code>false</code>
     * @see Calculator::optimize
     */
    virtual bool isVolatile() const;
};


class Calculator
{
    enum class NodeType : unsigned char
    {
        VALUE,
        FUNCTION_2D,
        FUNCTION_3D,
        ADDITION,
        SUBTRACTION,
        DIVISION,
        MULTIPLICATION,
        MODULO,
        INTEGER_DIVISION
    };

    enum class SyntaxError : unsigned char{
        UNCLOSED_PARENTHESES,
        UNEXPECTED_CHARACTER,
        UNKNOWN_FUNCTION,
        INVALID_ARGUMENTS_NUMBER
    };


    struct Node
    {
        NodeType type;
        double value;

        Node *left;
        Node *right;

        Node()
        {
            left = nullptr;
            right = nullptr;
        }

        virtual ~Node();
    };


    struct Function2DNode : Node
    {
        Node *arg;
        CalculatorFunction2D *func;


        Function2DNode();
    };


    struct Function3DNode : Node
    {
        Node *arg1;
        Node *arg2;
        CalculatorFunction3D *func;


        Function3DNode();
    };

    struct SyntaxInfo {
        bool ok;
        SyntaxError what;
        const char *where;
    };

    Node *_expr;
    std::map<QString, CalculatorFunction2D*> _functions2D;
    std::map<QString, CalculatorFunction3D*> _functions3D;

    Node *parse(const char *expr, int length, SyntaxInfo &syntaxResult);
    void countNode(Node *node);
    void optimize(Node *root);
public:
    Calculator();

    ~Calculator();

    void parseExpression(QString str);

    double value();

    void installFunction2D(CalculatorFunction2D *func, bool force = false);

    void installFunction3D(CalculatorFunction3D *func, bool force = false);
};


}


#endif // CALCULATOR_HPP
