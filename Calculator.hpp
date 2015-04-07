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
 * The <code>CalculatorFunction2D</code> class is basic class for calculator's functions with one parameter.
 * Pointer to <code>CalculatorFunction2D</code> may be instelled in calculator. Then it can be interpreted in formulas using syntax <code>name(expr)</code>, where <code>name</code> is function's name and
 * <code>expr</code> is an formulas witch value is given to function <code>CalculatorFunction2D::valueOf</code>. For example this class represents function called <code>foo</code>, witch for given number
 * <code>x</code> returns number <code>2*x+1</code>. Exemplary formula that uses it would be <code>foo(3+foo(0))+1</code>.
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
     * The <code>name</code> function resurns name that is used to call it in calculator's formula.
     * Calculator uses value returned by this function to recongize it calls in formula it parses. When doing it, the calculator is case sensitive. It must consist of letters (<code>isalpha</code>
     * function mustr return <code>true</code> for all letters in name).
     * @return function's name used to recongize it calls (case sensitive). In this case name is <code>"foo"</code>.
     */
    virtual QString name() const;

    /**
     * The <code>valueOf</code> is call to this function for argument <code>arg</code>.
     * During computing value of formula Calculator will use this function to ask, what will be its for given argument. Here <code>valueOf</code> returns <code>2*arg+1</code>, so calculator
     * with formula for example <code>foo(3)+1</code> will return <code>(2*3+1)+1=8</code>.
     * @param arg argument for function
     * @return function's value for given argument. In this case <code>2*arg+1</code>
     */
    virtual double valueOf(double arg);

    /**
     * The <code>isVolatile</code> is used to determine if function's value returned from constant formula is constant.
     * For example this class' function <code>valueOf(arg)</code> retruns <code>2*arg+1</code>. Consequently <code>foo(3)</code> always will be equal <code>7</code> – it is not volatile. In
     * contrary function <code>Scripting::FunctionOffset</code>'s result depends on cell position selected bay automaton. Therefore <code>OFFSET(-1;-1)</code> (state of up-left neighbour of
     * selected cell) will return differtnt values even if it has constant arguments – it is volatile. This will not let Calculator treat it as constant value during optimization.
     * @return <code>true</code> if and only if function is volatile. In such case returns <code>false</code>
     * @see Calculator::optimize
     */
    virtual bool isVolatile() const;
};



/**
 * The <code>CalculatorFunction3D</code> class is basic class for calculator's functions with twoe parameters.
 * Pointer to <code>CalculatorFunction3D</code> may be instelled in calculator. Then it can be interpreted in formula using syntax <code>name(expr; expr)</code>, where <code>name</code> is
 * function's name and <code>expr</code> are formula witch value is given to function <code>CalculatorFunction3D::valueOf</code>. For example this class represents function called
 * <code>fooo</code>, witch for given numbers <code>x</code> and <code>y</code> returns number <code>x*y</code>. Exemplary formula that uses it would be <code>fooo(1+2;fooo(1;1))</code>.
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
     * The <code>name</code> function resurns name that is used to call it in calculator's formula.
     * Calculator uses value returned by this function to recongize it calls in formula it parses. When doing it, the calculator is case sensitive. It must consist of letters (<code>isalpha</code>
     * function mustr return <code>true</code> for all letters in name).
     * @return function's name used to recongize it calls (case sensitive). In this case name is <code>"fooo"</code>.
     */
    virtual QString name() const;

    /**
     * The <code>valueOf</code> is call to this function for arguments <code>arg1</code> and <code>arg2</code>.
     * During computing value of formula Calculator will use this function to ask, what will be its for given argument. Here <code>valueOf</code> returns <code>arg1*arg2</code>, so calculator
     * with formula for example <code>fooo(1+2;fooo(1;1))</code> will return <code>fooo(3;1)=3*1=3</code>.
     * @param arg1 first argument for function
     * @param arg2 second argument for function
     * @return function's value for given arguments. In this case <code>arg1*arg2</code>
     */
    virtual double valueOf(double arg1, double arg2);

    /**
     * The <code>isVolatile</code> is used to determine if function's value returned from constant formula is constant.
     * For example class <code>Scripting::CalculatorFunction2D</code>'s function <code>valueOf(arg)</code> retruns <code>2*arg+1</code>. Consequently <code>foo(3)</code> always will be equal
     * <code>7</code> – it is not volatile. In contrary function <code>Scripting::FunctionOffset</code>'s result depends on cell position selected bay automaton. Therefore <code>OFFSET(-1;-1)</code>
     * (state of up-left neighbour of selected cell) will return differtnt values even if it has constant arguments – it is volatile. This will not let Calculator treat it as constant value
     * during optimization.
     * @return <code>true</code> if and only if function is volatile. In such case returns <code>false</code>
     * @see Calculator::optimize
     */
    virtual bool isVolatile() const;
};



/**
 * The <code>Calculator</code> class parses formula given in string and computes its value.
 * Formula is parsed to tree Each node in such tree can be arithmetic operation or function and each leaf is a constant value. The children of node are arguments for function or operator.
 * <code>Calculator</code> can be used to check multiple times value of expression – as in <code>CellularAutomaton</code> where the same rule is applicated multiple times for different cells.
 *
 * In formulas there are six defined operators: addition ('+'), subtraction ('-'), multiplication ('*'), division ('/'), modulo ('%') and integer division (':'). Expression given to
 * <code>parseExpression</code> function cannot contain spaces. Braces are supported. Semiclon separates function's (if more then one) arguments. Values after unused semiclons are ignored –
 * exemplary value of expression <code>"19+8;0-15/2"</code> would be 27.
 *
 * @see CalculatorFunction2D
 * @see CalculatorFunction3D
 */
class Calculator
{
    /**
     * The <code>NodeType</code> enum is used in <code>Node</code> structure to mark the oparation witch shall be preformed on its children.
     * @see Node
     */
    enum class NodeType : unsigned char
    {
        VALUE, ///< Nodes contains constant value
        FUNCTION_2D, ///< Nodes is instance of <code>Function2DNode</code> and it calls function to get value of its argument
        FUNCTION_3D, ///< Nodes is instance of <code>Function3DNode</code> and it calls function to get value of its arguments
        ADDITION, ///< Value of node will be sum of values of it's sons. Operator.
        SUBTRACTION, ///< Value of node will be <code>s1-s2</code> where <code>s1</code> is value of first son and <code>s2</code> is value of the second one. Operator.
        DIVISION, ///< Value of node will be <code>s1/s2</code> where <code>s1</code> is value of first son and <code>s2</code> is value of the second one. Operator.
        MULTIPLICATION, ///< Value of node will be product of values of it's sons. Operator.
        MODULO, ///< Value of node will be <code>s1%s2</code> where <code>s1</code> is value of first son and <code>s2</code> is value of the second one. As <code>s1</code> and <code>s2</code>
                ///< must be integer, thrir not integer part is ignored. Operator.
        INTEGER_DIVISION ///< Value of node will be <code>floor(s1/s2)</code> where <code>s1</code> is value of first son and <code>s2</code> is value of the second one. Operator.
    };


    /**
     * The <code>SyntaxError</code> enum provides information about syntax error type in given expression
     */
    enum class SyntaxError : unsigned char {
        UNCLOSED_PARENTHESES, ///< There is something wrong with braces
        UNEXPECTED_CHARACTER, ///< Unallowed character was found in expression
        UNKNOWN_FUNCTION, ///< Given function's name is not found in function's register
        INVALID_ARGUMENTS_NUMBER ///< Number of arguments supplied to functioin is incorrect (don't works)
    };


    /**
     * The <code>Node</code> is basic node in tree used for computing values of formulas
     * All operators nodes in tree should have exactly two sons (left and right). Constant values don't have sons. Same for function's nodes – arguments are stored in different variables.
     */
    struct Node
    {
        /**
         * type indicates how the node should be treated
         */
        NodeType type;

        /**
         * value current value of this node.
         * For constant nodes is stays olways the same. For all others it is set during calculating value of formula.
         * @see Calculator::value
         */
        double value;

        /**
         * left argument of operator
         * Pointer to node witch is left argument of operator in current node.
         * @see Node::right
         * @see NodeType
         */
        Node *left;

        /**
         * right argument of operator
         * Pointer to node witch is right argument of operator in current node. <code>nullptr</code> when thi node is not operator
         * @see Node::left
         * @see NodeType
         */
        Node *right;


        /**
         * Node's constructor
         * This constructor initializes <code>left</code> and <code>right</code> with <code>nullptr</code>.
         */
        Node();

        /**
         * Node's destructor
         * This destructor deletes left and right nodes – it is called recursively.
         */
        virtual ~Node();
    };


    /**
     * The <code>Function2DNode</code> is node witch references to function
     * This structure is representing a call to calculator's function.
     */
    struct Function2DNode : Node
    {
        /**
         * <code>arg</code> is a pointer to subtree with parsed expression given as function's argument.
         */
        Node *arg;

        /**
         * <code>func</code> is a pointer to function called by this node.
         */
        CalculatorFunction2D *func;

        /**
         * Function2DNode's constructor
         * This constructor initializes <code>arg</code> and <code>func</code> with <code>nullptr</code>.
         */
        Function2DNode();
    };


    /**
     * The <code>Function2DNode</code> is node witch references to function
     * This structure is representing a call to calculator's function.
     */
    struct Function3DNode : Node
    {
        /**
         * <code>arg</code> is a pointer to subtree with parsed expression given as function's first argument.
         */
        Node *arg1;

        /**
         * <code>arg</code> is a pointer to subtree with parsed expression given as function's frist argument.
         */
        Node *arg2;

        /**
         * <code>func</code> is a pointer to function called by this node.
         */
        CalculatorFunction3D *func;

        /**
         * Function3DNode's constructor This constructor initializes <code>arg1</code>, <code>arg2</code> and <code>func</code> with <code>nullptr</code>.
         */
        Function3DNode();
    };

    /**
     * The SyntaxInfo struct is returned by function Calculator::parse(). It is used to pass informations about any sntax errors.
     */
    struct SyntaxInfo {
        /**
         * <code>ok</code> is true if and only if there are not syntax errors.
         */
        bool ok;

        /**
         * <code>what</code> contains error's type, if any occured.
         */
        SyntaxError what;

        /**
         * <code>what</code> contains pointer to character, where error was reported.
         */
        const char *where;
    };

    /**
     * <code>_source</code> stores string witch was source for parser expression.
     */
    QString _source;

    /**
     * <code>_expr</code> is pointer to root node of parsed expression's tree.
     */
    Node *_expr;

    /**
     * <code>_functions2D</code> contains pointers to all one argument function known to this automaton, mapped by name.
     */
    std::map<QString, CalculatorFunction2D*> _functions2D;

    /**
     * <code>_functions3D</code> contains pointers to all two argument function known to this automaton, mapped by name.
     */
    std::map<QString, CalculatorFunction3D*> _functions3D;

    /**
     * The <code>parse</code> function parses recursively given expression and returns parsed tree.
     * @param expr pointer to string with expression.
     * @param length length of expression
     * @param syntaxResult a place to put syntax check result
     * @return pointer to root of parsed tree; <code>nullptr</code> on failure
     * @throw Exceptions::IllegalArgumentException when <code>length < 1</code>
     */
    Node *parse(const char *expr, int length, SyntaxInfo &syntaxResult);

    /**
     * Function <code>countNode</code> computes value of given node. It works only for operators. Value is computed using <code>value</code> field in chindren, and it is
     * written to <code>value</code> in given node.
     * @param node value of this node will be computed.
     * @throw Exceptions::ArithmeticErrorException when right argument is equal 0 and operator is modulo, or any division operation.
     * @note Operation <code>5%0.5</code> will also throw exception, because modulo works only with integers and <code>static_cast<int>(0.5) == 0</code>
     */
    void countNode(Node *node);

    /**
     * Makes simple optimizations on tree with root in given node. Firstly is preforms the same optimisations recursively on its children.
     * If given node is an operator, this function simply replaces it with constant value, if both arguments are constant. If it is function, node can be optimizes only if
     * it's argument is constant and function is not volatile.
     * @param root
     */
    void optimize(Node *root);

public:
    Calculator();

    ~Calculator();

    /**
     * Parses formula and stores it in the object. Previously stored expression is deleted before the new one is parsed so the previous expression is deleted even if new is invalid
     * (syntax error etc). After parsing succesfully, the expression is optimized. If there were errors during parsing, <code>Exceptions::SyntaxErrorException</code> is thrown.
     * @param str <code>QString</code> with formula
     */
    void parseExpression(QString str);

    /**
     * Calculates value of expression or NAN if no expression was parsed, or it couldn't be barsed (for exemple because of syntax errors). On arythmetic error (such as division by 0),
     * <code>Exceptions::ArithmeticErrorException</code> is thrown. Other exceptions may occur, if they are thrown by installed functions.
     * @return Value of parsed expression or NAN if there is not parsed expression.
     * @throw Exceptions::ArithmeticErrorException when value cannot be computed because of division by 0
     * @warning Other exceptions may be thrown from installed functions
     */
    double value();

    /**
     * Adds function with one argument to list of known functions. If function with such name was already added, exception will be thrown (unless force is true).
     * This function affects only on the one instance of <code>Calculator</code> class.
     * @param func pointer to nstance of class derriving form <code>FunctionD2Instance</code> – function witch is going to be added.
     * @param force if function with the same name already occurs in registry, it will not be overriden unless this variable is <code>true</code>
     */
    void installFunction2D(CalculatorFunction2D *func, bool force = false);

    /**
     * Adds function with two arguments to list of known functions. If function with such name was already added, exception will be thrown (unless force is true).
     * This function affects only on the one instance of <code>Calculator</code> class.
     * @param func pointer to nstance of class derriving form <code>FunctionD3Instance</code> – function witch is going to be added.
     * @param force if function with the same name already occurs in registry, it will not be overriden unless this variable is <code>true</code>
     */
    void installFunction3D(CalculatorFunction3D *func, bool force = false);

    /**
     * @return source string for last succesfuly parsed expression or empty string if there were no expressions successfuly parsed.
     */
    QString toString() const;
};


}


#endif // CALCULATOR_HPP
