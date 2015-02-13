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

namespace Scripting
{

class CalculatorFunction2D
{
public:
    virtual std::string name() const;
    virtual double valueOf(double arg) const;
    virtual bool isVolatile();
};



class CalculatorFunction3D
{
public:
    virtual std::string name() const;
    virtual double valueOf(double arg1, double arg2) const;
    virtual bool isVolatile();
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

        virtual ~Node()
        {
            if (left != nullptr)
                delete left;
            if (right != nullptr)
                delete right;
        }
    };


    struct Function2DNode : Node
    {
        Node *arg;
        CalculatorFunction2D *func;


        Function2DNode() :
            Node()
        {
        }
    };


    struct Function3DNode : Node
    {
        Node *arg1;
        Node *arg2;
        CalculatorFunction3D *func;


        Function3DNode() :
            Node()
        {
        }
    };

    Node *_expr;
    std::map<std::string, CalculatorFunction2D*> functions2D;
    std::map<std::string, CalculatorFunction3D*> functions3D;

    Node *parse(const char *expr, int length) throw (std::string)
    {
        Node *result = nullptr;

        auto reportParseError = [&](const char *ptr, const char *msg) {
            if (result != nullptr)
                delete result;
            char buf[100];
            snprintf(buf, 100, "Parse error at %p – %s", static_cast<const void *>(ptr), msg);
            throw std::string(buf);
        };

        if (length < 1)
            reportParseError(expr, "empty expression");

        int lowestPriorityIndex = -1;
        int lowestPriority = -1;
        int priority;
        int braces = 0;
        for (int i = length - 1; i >= 0; --i) {
            priority = 2 * braces;
            bool isOperator = false;
            if (!isalnum(expr[i]) && expr[i] != '.' && expr[i] != ';') {
                switch (expr[i]) {
                    case '(':
                        --braces;
                        break;
                    case ')':
                        ++braces;
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
                        reportParseError(expr + 1, "unknown operator");
                }

                if ((priority < lowestPriority || lowestPriority == -1) && isOperator) {
                    lowestPriority = priority;
                    lowestPriorityIndex = i;
                }

                if (lowestPriority == 0)
                    break;
            }

            if (braces < 0) {
                reportParseError(expr + i, "negative braces");
            }
        }

        if (braces != 0)
            reportParseError(expr + length - 1, "unknown operator");

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
            try {
                result->left = parse(expr, lowestPriorityIndex);
                result->right = parse(expr + lowestPriorityIndex + 1, length - lowestPriorityIndex - 1);
            } catch (int except) {
                if (result != nullptr)
                    delete result;
                throw except;
            }
        } else {
            if (isdigit(expr[0])) {
                result = new Node;
                result->value = atof(expr);
                result->type = Calculator::NodeType::VALUE;
            } else {
                int i = 0;

                std::string functionsName;
                for (; expr[i] != '(' && i < length; ++i) {
                    if (!isalpha(expr[i]))
                        reportParseError(expr + i, "invalid function name");
                    else
                        functionsName += expr[i];
                }

                if (i == length)
                    reportParseError(expr + i, "expected braces and argument");
                ++i;

                if (functions2D.find(functionsName) != functions2D.end()) {
                    Function2DNode *newNode = new Function2DNode;
                    newNode = new Function2DNode;
                    newNode->type = Calculator::NodeType::FUNCTION_2D;
                    newNode->func = functions2D[functionsName];

                    newNode->arg = parse(expr + i, length - i - 1);
                    result = newNode;
                } else if (functions3D.find(functionsName) != functions3D.end()) {
                    Function3DNode *newNode = new Function3DNode;
                    newNode = new Function3DNode;
                    newNode->type = Calculator::NodeType::FUNCTION_3D;
                    newNode->func = functions3D[functionsName];

                    int semiclonPosition = i;
                    for (int braces = 0; semiclonPosition < length - 1; ++semiclonPosition) {
                        switch (expr[semiclonPosition]) {
                        case '(':
                            ++braces;
                            break;
                        case ')':
                            --braces;
                            break;
                        }
                        if (expr[semiclonPosition] == ';' && braces == 0)
                            break;
                    }
                    if (semiclonPosition == length - 1)
                        reportParseError(expr + semiclonPosition - 1, "to few arguments");

                    std::cerr << length << " " << i << " " << semiclonPosition << std::endl;
                    newNode->arg1 = parse(expr + i, semiclonPosition - i);
                    newNode->arg2 = parse(expr + semiclonPosition + 1, length - semiclonPosition - 2);
                    result = newNode;
                } else {
                    reportParseError(expr + i, "no such function");
                }
            }
        }

        return result;
    }


    void countNode(Node *node) throw (std::string)
    {
        switch (node->type) {
            case Calculator::NodeType::ADDITION:
                node->value = node->left->value + node->right->value;
                break;
            case Calculator::NodeType::SUBTRACTION:
                node->value = node->left->value - node->right->value;
                break;
            case Calculator::NodeType::DIVISION:
                if (node->right->value == 0)
                    throw std::string("Division by 0");
                node->value = node->left->value / node->right->value;
                break;
            case Calculator::NodeType::MULTIPLICATION:
                node->value = node->left->value * node->right->value;
                break;
            case Calculator::NodeType::INTEGER_DIVISION:
                if (node->right->value == 0)
                    throw std::string("Division by 0");
                node->value = std::floor(node->left->value / node->right->value);
                break;
            case Calculator::NodeType::MODULO:
                if (static_cast<int>(node->right->value) == 0)
                    throw std::string("Division by 0");
                node->value = static_cast<int>(node->left->value) % static_cast<int>(node->right->value);
                break;
        }
    }


    void optimize(Node *root) throw (std::string)
    {
        if (root->type == Calculator::NodeType::FUNCTION_2D) {
            Function2DNode *convertedRoot = dynamic_cast<Function2DNode*>(root);
            if (convertedRoot == nullptr)
                    throw std::string("Corrupted expression's tree!");

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
                    throw std::string("Corrupted expression's tree!");

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

public:
    Calculator()
    {
        _expr = nullptr;
    }


    ~Calculator()
    {
        if (_expr != nullptr) {
            delete _expr;
            _expr = nullptr;
        }
    }


    void parseExpression(std::string str) throw (std::string)
    {
        if (_expr != nullptr) {
            delete _expr;
            _expr = nullptr;
        }
        _expr = parse(str.c_str(), str.length());
        optimize(_expr);
    }


    double value() throw (std::string)
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
                    throw std::string("Corrupted expression's tree!");
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
                    throw std::string("Corrupted expression's tree!");
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


    void bindFunction2D(CalculatorFunction2D *func, bool force = false) throw (std::string)
    {
        if (functions2D.find(func->name()) != functions2D.end() && !force)
            throw std::string("Register already contains function with such name");
        functions2D[func->name()] = func;
    }


    void bindFunction3D(CalculatorFunction3D *func, bool force = false) throw (std::string)
    {
        if (functions3D.find(func->name()) != functions3D.end() && !force)
            throw std::string("Register already contains function with such name");
        functions3D[func->name()] = func;
    }
};


}


#endif // CALCULATOR_HPP
