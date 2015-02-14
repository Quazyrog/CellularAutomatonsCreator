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
 * @brief The CalculatorFunction2D class is calculator's function with one parameter.
 *
 */
class CalculatorFunction2D
{
public:
    virtual QString name() const;
    virtual double valueOf(double arg) throw (Exceptions::ArithmeticErrorException);
    virtual bool isVolatile() const;
};



class CalculatorFunction3D
{
public:
    virtual QString name() const;
    virtual double valueOf(double arg1, double arg2) throw (Exceptions::ArithmeticErrorException);
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
        Exceptions::MathSyntaxErrorException::Error what;
        const char *where;
    };

    Node *_expr;
    std::map<QString, CalculatorFunction2D*> _functions2D;
    std::map<QString, CalculatorFunction3D*> _functions3D;

    Node *parse(const char *expr, int length, SyntaxInfo &syntaxResult)
    throw (Exceptions::IllegalArgumentException);

    void countNode(Node *node)
    throw (Exceptions::ArithmeticErrorException, Exceptions::NullPointerException, Exceptions::IllegalArgumentException);

    void optimize(Node *root)
    throw (Exceptions::RuntimeException);

public:
    Calculator();

    ~Calculator();

    void parseExpression(QString str) throw (Exceptions::IllegalArgumentException);

    double value()
    throw (Exceptions::ArithmeticErrorException, Exceptions::RuntimeException);

    void bindFunction2D(CalculatorFunction2D *func, bool force = false)
    throw (Exceptions::RuntimeException);

    void bindFunction3D(CalculatorFunction3D *func, bool force = false)
    throw (Exceptions::RuntimeException);
};


}


#endif // CALCULATOR_HPP
