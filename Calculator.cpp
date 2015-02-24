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

#include "Calculator.hpp"

namespace Scripting
{

QString CalculatorFunction2D::name() const
{
    std::cerr << "Virtual function CalculatorFunction2D::name shouldn't be called." << std::endl;
    return QString("foo");
}


double CalculatorFunction2D::valueOf(double arg)
{
    std::cerr << "Virtual function CalculatorFunction2D::valueOf shouldn't be called." << std::endl;
    return 2 * arg + 1;
}


bool CalculatorFunction2D::isVolatile() const
{
    return false;
}


QString CalculatorFunction3D::name() const
{
    qWarning () << "Virtual function CalculatorFunction3D::name shouldn't be called.";
    return QString("foo");
}


double CalculatorFunction3D::valueOf(double arg1, double arg2)
{
    qWarning() << "Virtual function CalculatorFunction3D::valueOf shouldn't be called.";
    return arg2 * arg1;
}


bool CalculatorFunction3D::isVolatile() const
{
    return false;
}


Calculator::Node::~Node()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}


Calculator::Function2DNode::Function2DNode():
    Node()
{
    arg = nullptr;
    func = nullptr;
}


Calculator::Function3DNode::Function3DNode():
    Node()
{
    arg1 = nullptr;
    arg2 = nullptr;
    func = nullptr;
}


Calculator::Calculator()
{
    _expr = nullptr;
}

Calculator::~Calculator()
{
    if (_expr != nullptr)
        delete _expr;
    _expr = nullptr;
}


Calculator::Node *Calculator::parse(const char *expr, int length, SyntaxInfo &syntaxResult)
{
    Node *result = nullptr;

    if (length < 1)
        throw Exceptions::IllegalArgumentException();

    int lowestPriorityIndex = -1;
    int lowestPriority = -1;
    int priority;
    int parentheses = 0;
    for (int i = length - 1; i >= 0; --i) {
        priority = 2 * parentheses;
        bool isOperator = false;
        if (!isalnum(expr[i]) && expr[i] != '.' && expr[i] != ';') {
            switch (expr[i]) {
                case '(':
                    --parentheses;
                    break;
                case ')':
                    ++parentheses;
                    break;
                case '%':
                case ':':
                case '*':
                case '/':
                    isOperator = true;
                    ++priority;
                    break;
                case '+':
                case '-':
                    isOperator = true;
                    break;
                default:
                    syntaxResult.ok = false;
                    syntaxResult.where = expr + i;
                    syntaxResult.what = SyntaxError::UNEXPECTED_CHARACTER;
                    return nullptr;
            }

            if ((priority < lowestPriority || lowestPriority == -1) && isOperator) {
                lowestPriority = priority;
                lowestPriorityIndex = i;
            }

            if (lowestPriority == 0)
                break;
        }

        if (parentheses < 0) {
            syntaxResult.ok = false;
            syntaxResult.where = nullptr;
            syntaxResult.what = SyntaxError::UNCLOSED_PARENTHESES;
            return nullptr;
        }
    }

    if (parentheses != 0) {
        syntaxResult.ok = false;
        syntaxResult.where = nullptr;
        syntaxResult.what = SyntaxError::UNCLOSED_PARENTHESES;
        return nullptr;
    }
    if (isalpha(expr[0]) && lowestPriority >= 2) {
        lowestPriority = -1;
        lowestPriorityIndex = -1;
    }

    if (lowestPriority != -1) {
        expr += lowestPriority >> 1;
        lowestPriorityIndex -= lowestPriority >> 1;
        length -= (lowestPriority >> 1) * 2;
    } else {
        while (expr[0] == '(' && expr[length - 1] == ')') {
            expr++;
            length -= 2;
        }
    }

    if (lowestPriority != -1) {
        result = new Node;
        switch (expr[lowestPriorityIndex]) {
            case '+':
                result->type = Calculator::NodeType::ADDITION;
                break;
            case '-':
                result->type = Calculator::NodeType::SUBTRACTION;
                break;
            case ':':
                result->type = Calculator::NodeType::INTEGER_DIVISION;
                break;
            case '%':
                result->type = Calculator::NodeType::MODULO;
                break;
            case '*':
                result->type = Calculator::NodeType::MULTIPLICATION;
                break;
            case '/':
                result->type = Calculator::NodeType::DIVISION;
                break;
        }

        result->left = parse(expr, lowestPriorityIndex, syntaxResult);
        if (result->left == nullptr) {
            delete result;
            return nullptr;
        }

        result->right = parse(expr + lowestPriorityIndex + 1, length - lowestPriorityIndex - 1, syntaxResult);
        if (result->right == nullptr) {
            delete result;
            return nullptr;
        }
    } else {
        if (isdigit(expr[0])) {
            result = new Node;
            result->value = atof(expr);
            result->type = Calculator::NodeType::VALUE;
        } else {
            int i = 0;

            QString functionsName;
            for (; expr[i] != '(' && i < length; ++i) {
                if (!isalpha(expr[i])) {
                    syntaxResult.ok = false;
                    syntaxResult.where = expr + i;
                    syntaxResult.what = SyntaxError::UNEXPECTED_CHARACTER;
                    return nullptr;
                } else {
                    functionsName += expr[i];
                }
            }

            if (i == length) {
                syntaxResult.ok = false;
                syntaxResult.where = expr + i;
                syntaxResult.what = SyntaxError::UNCLOSED_PARENTHESES;
                return nullptr;
            }
            ++i;

            if (_functions2D.find(functionsName) != _functions2D.end()) {
                Function2DNode *newNode = new Function2DNode;
                newNode = new Function2DNode;
                newNode->type = Calculator::NodeType::FUNCTION_2D;
                newNode->func = _functions2D[functionsName];

                newNode->arg = parse(expr + i, length - i - 1, syntaxResult);
                if (newNode->arg == nullptr) {
                    delete newNode;
                    return nullptr;
                }
                result = newNode;
            } else if (_functions3D.find(functionsName) != _functions3D.end()) {
                Function3DNode *newNode = new Function3DNode;
                newNode = new Function3DNode;
                newNode->type = Calculator::NodeType::FUNCTION_3D;
                newNode->func = _functions3D[functionsName];

                int semiclonPosition = i;
                for (int parentheses = 0; semiclonPosition < length - 1; ++semiclonPosition) {
                    switch (expr[semiclonPosition]) {
                    case '(':
                        ++parentheses;
                        break;
                    case ')':
                        --parentheses;
                        break;
                    }
                    if (expr[semiclonPosition] == ';' && parentheses == 0)
                        break;
                }
                if (semiclonPosition == length - 1) {
                    syntaxResult.ok = false;
                    syntaxResult.where = expr + semiclonPosition - 1;
                    syntaxResult.what = SyntaxError::INVALID_ARGUMENTS_NUMBER;
                    return nullptr;
                }

                newNode->arg1 = parse(expr + i, semiclonPosition - i, syntaxResult);
                newNode->arg2 = parse(expr + semiclonPosition + 1, length - semiclonPosition - 2, syntaxResult);
                result = newNode;
            } else {
                syntaxResult.ok = false;
                syntaxResult.where = expr + i;
                syntaxResult.what = SyntaxError::INVALID_ARGUMENTS_NUMBER;
                return nullptr;
            }
        }
    }

    return result;
}


void Calculator::countNode(Node *node)
{
    if (node->type == Calculator::NodeType::VALUE)
        return;

    if (node->left == nullptr || node->right == nullptr)
        throw Exceptions::NullPointerException();

    switch (node->type) {
    case Calculator::NodeType::ADDITION:
        node->value = node->left->value + node->right->value;
        break;
    case Calculator::NodeType::SUBTRACTION:
        node->value = node->left->value - node->right->value;
        break;
    case Calculator::NodeType::DIVISION:
        if (node->right->value == 0)
            throw Exceptions::ArithmeticErrorException(Exceptions::ArithmeticErrorException::DIVISION_BY_ZERO);
        node->value = node->left->value / node->right->value;
        break;
    case Calculator::NodeType::MULTIPLICATION:
        node->value = node->left->value * node->right->value;
        break;
    case Calculator::NodeType::INTEGER_DIVISION:
        if (node->right->value == 0)
            throw Exceptions::ArithmeticErrorException(Exceptions::ArithmeticErrorException::DIVISION_BY_ZERO);
        node->value = std::floor(node->left->value / node->right->value);
        break;
    case Calculator::NodeType::MODULO:
        if (static_cast<int>(node->right->value) == 0)
            throw Exceptions::ArithmeticErrorException(Exceptions::ArithmeticErrorException::DIVISION_BY_ZERO);
        node->value = static_cast<int>(node->left->value) % static_cast<int>(node->right->value);
        break;
    default:
        qWarning() << "Unexpected switch value!";
    }
}


double Calculator::value()
{
    if (_expr == nullptr)
        return NAN;

    struct StackEntry
    {
        char computed;
        Node *node;

        StackEntry(Node *ptr)
        {
            computed = 0;
            node = ptr;
        }
    };

    static std::stack<StackEntry> expressionsStack;

    expressionsStack.push(StackEntry(_expr));
    do {
        StackEntry &frame = expressionsStack.top();

        if (frame.node->type == Calculator::NodeType::VALUE) {
            expressionsStack.pop();
        } else if (frame.node->type == Calculator::NodeType::FUNCTION_2D) {
            Function2DNode *node = dynamic_cast<Function2DNode*>(frame.node);
            if (node == nullptr)
                throw Exceptions::RuntimeException();
            if (frame.computed == 0) {
                expressionsStack.push(node->arg);
                frame.computed++;
            } else {
                node->value = node->func->valueOf(node->arg->value);
                expressionsStack.pop();
            }
        } else if (frame.node->type == Calculator::NodeType::FUNCTION_3D) {
            Function3DNode *node = dynamic_cast<Function3DNode*>(frame.node);
            if (node == nullptr)
                throw Exceptions::RuntimeException();
            if (frame.computed == 0) {
                expressionsStack.push(node->arg1);
                frame.computed++;
            } else if (frame.computed == 1) {
                expressionsStack.push(node->arg2);
                frame.computed++;
            } else {
                node->value = node->func->valueOf(node->arg1->value, node->arg2->value);
                expressionsStack.pop();
            }
        } else {
            if (frame.computed == 0) {
                expressionsStack.push(frame.node->left);
                frame.computed++;
            } else if (frame.computed == 1) {
                expressionsStack.push(frame.node->right);
                frame.computed++;
            } else {
                countNode(frame.node);
                expressionsStack.pop();
            }
        }
    } while (expressionsStack.size() > 0);

    return _expr->value;
}


void Calculator::optimize(Node *root)
{
    if (root->type == Calculator::NodeType::FUNCTION_2D) {
        Function2DNode *convertedRoot = dynamic_cast<Function2DNode*>(root);
        if (convertedRoot == nullptr)
                throw Exceptions::RuntimeException();

        optimize(convertedRoot->arg);
        if (convertedRoot->arg->type == Calculator::NodeType::VALUE
            && !convertedRoot->func->isVolatile()) {
            convertedRoot->type = Calculator::NodeType::VALUE;
            convertedRoot->value = convertedRoot->func->valueOf(convertedRoot->arg->value);
        }
        return;
    }

    if (root->type == Calculator::NodeType::FUNCTION_3D) {
        Function3DNode *convertedRoot = dynamic_cast<Function3DNode*>(root);
        if (convertedRoot == nullptr)
                throw Exceptions::RuntimeException();

        optimize(convertedRoot->arg1);
        optimize(convertedRoot->arg2);
        if (convertedRoot->arg1->type == Calculator::NodeType::VALUE
            && convertedRoot->arg2->type == Calculator::NodeType::VALUE
            && !convertedRoot->func->isVolatile()) {
            convertedRoot->type = Calculator::NodeType::VALUE;
            convertedRoot->value = convertedRoot->func->valueOf(convertedRoot->arg1->value, convertedRoot->arg2->value);
        }
        return;
    }

    if (root->left == nullptr || root->right == nullptr)
        return;

    optimize(root->left);
    optimize(root->right);
    if (root->left->type == Calculator::NodeType::VALUE && root->right->type == Calculator::NodeType::VALUE) {
        countNode(root);
        root->type = Calculator::NodeType::VALUE;
        delete root->left;
        root->left = nullptr;
        delete root->right;
        root->right = nullptr;
    }
}


void Calculator::parseExpression(QString str)
{
    if (_expr != nullptr) {
        delete _expr;
        _expr = nullptr;
    }
    SyntaxInfo si;
    si.ok = true;
    std::string s = str.toStdString();
    const char *expr = s.c_str();
    _expr = parse(expr, str.length(), si);
    if (!si.ok) {
        switch (si.what) {
        case SyntaxError::INVALID_ARGUMENTS_NUMBER:
            throw Exceptions::SyntaxErrorException(0, QString().sprintf("Invalid number of arguments given to fuction (near %lith character)", si.where - expr));
        case SyntaxError::UNCLOSED_PARENTHESES:
            throw Exceptions::SyntaxErrorException(0, QString().sprintf("Unclosed parentheses (near %lith character)", si.where - expr));
        case SyntaxError::UNEXPECTED_CHARACTER:
            throw Exceptions::SyntaxErrorException(0, QString().sprintf("Unexpected character (near %lith character)", si.where - expr));
        case SyntaxError::UNKNOWN_FUNCTION:
            throw Exceptions::SyntaxErrorException(0, QString().sprintf("Unknown function (near %lith character)", si.where - expr));
        }
    }
    optimize(_expr);

    _source = str;
}


void Calculator::installFunction2D(CalculatorFunction2D *func, bool force)
{
    if (_functions2D.find(func->name()) != _functions2D.end() && !force)
        throw Exceptions::RuntimeException();
    _functions2D[func->name()] = func;
}


void Calculator::installFunction3D(CalculatorFunction3D *func, bool force)
{
    if (_functions3D.find(func->name()) != _functions3D.end() && !force)
        throw Exceptions::RuntimeException();
    _functions3D[func->name()] = func;
}

QString Calculator::toString() const
{
    return _source;
}

} //Namespace Scripting

